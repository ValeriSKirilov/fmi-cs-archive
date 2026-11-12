"""Споделен помощен модул за Упражнение 3. НЕ го редактирате.

Същият `chat(...)` като в Упражнение 2, плюс един нов параметър: `schema`.
Подадете ли Pydantic модел, моделът е ПРИНУДЕН да върне JSON по тази схема.
Начинът, по който това се постига, се различава коренно между доставчиците.
Затова живее ТУК, на едно място:

  * Ollama  -> `format=<JSON схема>`; llama.cpp ограничава декодирането с
              граматика, тоест моделът физически не може да изведе токен
              извън схемата.
  * Anthropic -> описваме „инструмент“ със схемата като `input_schema` и
              принуждаваме модела да го извика (`tool_choice`); отговорът
              идва в `tool_use` блок. Tool calling, използван като structured
              output (пълно разглеждане в Тема 9).

И в двата случая `reply.text` е JSON низ, готов за `Model.model_validate_json`.

Проверка на средата (едно извикване, ~10 s):
    python llm.py

Какво е скрито тук (прегледайте го набързо, за да знаете какво ви е спестено):
  * четенето на `.env` и избора на доставчик/модел;
  * къде отива `system` (Ollama: съобщение с роля; Anthropic: отделен параметър);
  * как се казва таванът на изхода (`num_predict` / `max_tokens`) и как
    разбираме, че отговорът е отрязан;
  * кой модел приема `temperature`;
  * как се задава схема при всеки доставчик (вж. по-горе).
"""

from __future__ import annotations

import argparse
import json
import os
import sys
import time
from dataclasses import dataclass
from pathlib import Path

from dotenv import load_dotenv

CODE_DIR = Path(__file__).resolve().parent
load_dotenv(CODE_DIR / ".env")  # code/.env, независимо откъде е стартиран скриптът

PROVIDERS = ("ollama", "anthropic")

# bggpt:4b е моделът от Упражнение 1: BgGPT (INSAIT), адаптиран за български.
# За пускане с определен модел: --model llama3.2:1b (бърз, но пише лошо на български).
DEFAULT_MODELS = {"ollama": "bggpt:4b", "anthropic": "claude-haiku-4-5"}
OLLAMA_SOURCES = {"bggpt:4b": "hf.co/INSAIT-Institute/BgGPT-Gemma-3-4B-IT-GGUF:Q4_K_M"}

# Кои Anthropic модели още приемат temperature/top_p. Най-новите (Sonnet/Opus
# 5.x) връщат 400, а SDK 1.x приема полето само през extra_body. Това знание остарява
ANTHROPIC_SAMPLING = {"claude-haiku-4-5"}


@dataclass
class Reply:
    text: str
    input_tokens: int
    output_tokens: int
    seconds: float
    truncated: bool  # отрязан ли е отговорът от тавана на изхода (max_tokens)
    label: str  # "доставчик/модел", за реда с usage


def default_provider() -> str:
    return os.environ.get("LLM_PROVIDER", "ollama")


def model_for(provider: str) -> str:
    return os.environ.get(f"{provider.upper()}_MODEL", DEFAULT_MODELS[provider])


def cli(description: str) -> argparse.ArgumentParser:
    """Парсър с общите флагове --provider и --model; задачите добавят своите."""
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("--provider", choices=PROVIDERS, default=default_provider())
    parser.add_argument("--model", default=None,
                        help="по подразбиране от .env; напр. llama3.2:1b")
    return parser


def chat(
    system: str,
    messages: list[dict],
    *,
    max_tokens: int = 512,
    temperature: float | None = None,
    top_p: float | None = None,
    seed: int | None = None,
    stop: list[str] | None = None,
    schema: type | None = None,
    provider: str | None = None,
    model: str | None = None,
) -> Reply:
    """Едно извикване на модела. `messages` съдържа само user/assistant;
    `system` се подава отделно и се слага там, където доставчикът го очаква.

    `schema` е Pydantic модел. Подаден ли е, отговорът е гарантирано JSON по
    тази схема и `reply.text` е готов за `Model.model_validate_json(...)`.
    """
    provider = provider or default_provider()
    model = model or model_for(provider)
    start = time.perf_counter()
    if provider == "ollama":
        text, tin, tout, truncated = _ollama(system, messages, max_tokens,
                                             temperature, top_p, seed, stop, schema, model)
    elif provider == "anthropic":
        text, tin, tout, truncated = _anthropic(system, messages, max_tokens,
                                                temperature, top_p, seed, stop, schema, model)
    else:
        sys.exit(f"Непознат доставчик: {provider}. Изберете от {PROVIDERS}.")
    return Reply(text.strip(), tin, tout, time.perf_counter() - start, truncated,
                 f"{provider}/{model}")


def print_usage(reply: Reply) -> None:
    """Редът с токени и латентност, който печатат всички задачи."""
    print(f"   [{reply.label}  вход={reply.input_tokens} изход={reply.output_tokens}"
          f"  ·  {reply.seconds:.2f}s]")


# --- Доставчици (тук живеят разликите) --------------------------------------

def _ollama(system, messages, max_tokens, temperature, top_p, seed, stop, schema, model):
    import ollama

    options: dict = {"num_predict": max_tokens}  # таванът на изхода при Ollama
    for name, value in (("temperature", temperature), ("top_p", top_p),
                        ("seed", seed), ("stop", stop)):
        if value is not None:
            options[name] = value
    # Схемата се подава като `format` -> constrained decoding през граматика.
    extra = {"format": schema.model_json_schema()} if schema is not None else {}
    try:
        r = ollama.chat(
            model=model,
            # при Ollama system е обикновено съобщение с роля "system"
            messages=[{"role": "system", "content": system}, *messages],
            options=options,
            **extra,
        )
    except Exception as exc:
        source = OLLAMA_SOURCES.get(model, model)
        pull = f"ollama pull {source}" + (f"\n     ollama cp   {source} {model}"
                                         if source != model else "")
        sys.exit(f"Ollama: {exc}\n"
                 f"  -> пуснат ли е Ollama? (https://ollama.com, `ollama serve`)\n"
                 f"  -> изтеглен ли е моделът?\n     {pull}")
    return (r["message"]["content"], r.get("prompt_eval_count") or 0,
            r.get("eval_count") or 0, r.get("done_reason") == "length")


_warned: set[str] = set()


def _anthropic(system, messages, max_tokens, temperature, top_p, seed, stop, schema, model):
    import anthropic

    # Console ключ (x-api-key) или OAuth токен (Authorization: Bearer). Кой header
    # да ползва решава SDK-то; тук само даваме ясно съобщение вместо 401.
    if not (os.environ.get("ANTHROPIC_API_KEY") or os.environ.get("ANTHROPIC_AUTH_TOKEN")):
        sys.exit("Липсва ANTHROPIC_API_KEY. Копирайте .env.example в .env и го попълнете.")

    sampling = {k: v for k, v in (("temperature", temperature), ("top_p", top_p))
                if v is not None}
    extra = {}
    if sampling and model in ANTHROPIC_SAMPLING:
        extra = {"extra_body": sampling}  # SDK 1.x не описва полето в сигнатурата
    elif sampling and model not in _warned:
        # Не го крием: подаденият параметър се изпуска и потребителят трябва да го види.
        _warned.add(model)
        print(f"   [!] {model} не приема temperature/top_p, заявката минава без тях.",
              file=sys.stderr)
    if seed is not None:
        print("   [!] Anthropic няма seed, заявката минава без него.", file=sys.stderr)
    if stop:
        extra["stop_sequences"] = stop  # същото понятие, друго име

    if schema is not None:
        # Schema-constrained изход през tool calling: описваме инструмент,
        # чийто вход Е схемата, и принуждаваме модела да го ползва.
        extra["tools"] = [{
            "name": "record",
            "description": "Запиши извлечените данни.",
            "input_schema": schema.model_json_schema(),
        }]
        extra["tool_choice"] = {"type": "tool", "name": "record"}

    msg = anthropic.Anthropic().messages.create(
        model=model,
        max_tokens=max_tokens,
        system=system,  # при Anthropic system е отделен параметър
        messages=messages,
        **extra,
    )
    truncated = msg.stop_reason == "max_tokens"
    if schema is not None:
        block = next((b for b in msg.content if b.type == "tool_use"), None)
        text = json.dumps(block.input, ensure_ascii=False) if block else ""
    else:
        # При моделите с мислене първият блок може да е "thinking", затова
        # търсим изрично текстовия.
        text = next((b.text for b in msg.content if b.type == "text"), "")
    return (text, msg.usage.input_tokens, msg.usage.output_tokens, truncated)


# --- Проверка на средата: python llm.py ------------------------------------

if __name__ == "__main__":
    args = cli("Проверка на средата: едно кратко извикване.").parse_args()
    model = args.model or model_for(args.provider)
    print(f"Проверявам {args.provider}/{model} ...")
    reply = chat("Отговаряй с една дума.", [{"role": "user", "content": "Кажи „готово“."}],
                 max_tokens=10, provider=args.provider, model=model)
    print(f"Моделът отговори: {reply.text!r}")
    print_usage(reply)
    print("[ok] Средата е готова за упражнението.")
