"""Споделен помощен модул за Упражнение 2 — НЕ го редактирате.

Една функция `chat(...)`, която всички задачи ползват. Скриптовете не знаят
дали отговаря локалният Ollama, или Anthropic — това е малката версия на
адаптера от Упражнение 1 (Задача 4): разликите между доставчиците живеят ТУК,
на едно място, а не из задачите.

Проверка на средата (едно извикване, ~10 s):
    python llm.py

Какво е скрито тук (прегледайте го набързо, за да знаете какво ви е спестено):
  * четенето на `.env` и избора на доставчик/модел;
  * къде отива `system` (Ollama: съобщение с роля; Anthropic: отделен параметър);
  * как се казва таванът на изхода (`num_predict` / `max_tokens`) и как
    разбираме, че отговорът е отрязан;
  * кой модел приема `temperature` (вж. `demo_temperature.py` — показва се
    от преподавателя).
"""

from __future__ import annotations

import argparse
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
# 5.x) връщат 400, а SDK 1.x приема полето само през extra_body. Това знание
# остарява — затова е на ЕДНО място (вж. demo_temperature.py).
ANTHROPIC_SAMPLING = {"claude-haiku-4-5"}


@dataclass
class Reply:
    text: str
    input_tokens: int
    output_tokens: int
    seconds: float
    truncated: bool  # отрязан ли е отговорът от тавана на изхода (max_tokens)
    label: str  # "доставчик/модел" — за реда с usage


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
    provider: str | None = None,
    model: str | None = None,
) -> Reply:
    """Едно извикване на модела. `messages` съдържа само user/assistant;
    `system` се подава отделно и тук се слага там, където доставчикът го очаква."""
    provider = provider or default_provider()
    model = model or model_for(provider)
    start = time.perf_counter()
    if provider == "ollama":
        text, tin, tout, truncated = _ollama(system, messages, max_tokens,
                                             temperature, top_p, seed, stop, model)
    elif provider == "anthropic":
        text, tin, tout, truncated = _anthropic(system, messages, max_tokens,
                                                temperature, top_p, seed, stop, model)
    else:
        sys.exit(f"Непознат доставчик: {provider}. Изберете от {PROVIDERS}.")
    return Reply(text.strip(), tin, tout, time.perf_counter() - start, truncated,
                 f"{provider}/{model}")


def print_usage(reply: Reply) -> None:
    """Редът с токени и латентност, който печатат всички задачи."""
    print(f"   [{reply.label}  вход={reply.input_tokens} изход={reply.output_tokens}"
          f"  ·  {reply.seconds:.2f}s]")


# --- Доставчици (тук живеят разликите) --------------------------------------

def _ollama(system, messages, max_tokens, temperature, top_p, seed, stop, model):
    import ollama

    options: dict = {"num_predict": max_tokens}  # таванът на изхода при Ollama
    for name, value in (("temperature", temperature), ("top_p", top_p),
                        ("seed", seed), ("stop", stop)):
        if value is not None:
            options[name] = value
    try:
        r = ollama.chat(
            model=model,
            # при Ollama system е обикновено съобщение с роля "system"
            messages=[{"role": "system", "content": system}, *messages],
            options=options,
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


def _anthropic(system, messages, max_tokens, temperature, top_p, seed, stop, model):
    import anthropic

    if not os.environ.get("ANTHROPIC_API_KEY"):
        sys.exit("Липсва ANTHROPIC_API_KEY. Копирайте .env.example в .env и го попълнете.")

    sampling = {k: v for k, v in (("temperature", temperature), ("top_p", top_p))
                if v is not None}
    extra = {}
    if sampling and model in ANTHROPIC_SAMPLING:
        extra = {"extra_body": sampling}  # SDK 1.x не описва полето в сигнатурата
    elif sampling and model not in _warned:
        # Не го крием: подаденият параметър се изпуска и потребителят трябва да го види.
        _warned.add(model)
        print(f"   [!] {model} не приема temperature/top_p — заявката минава без тях.",
              file=sys.stderr)
    if seed is not None:
        print("   [!] Anthropic няма seed — заявката минава без него.", file=sys.stderr)
    if stop:
        extra["stop_sequences"] = stop  # същото понятие, друго име

    msg = anthropic.Anthropic().messages.create(
        model=model,
        max_tokens=max_tokens,
        system=system,  # при Anthropic system е отделен параметър
        messages=messages,
        **extra,
    )
    # При моделите с мислене първият блок може да е "thinking" — търсим текстовия.
    text = next((b.text for b in msg.content if b.type == "text"), "")
    return (text, msg.usage.input_tokens, msg.usage.output_tokens,
            msg.stop_reason == "max_tokens")


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
