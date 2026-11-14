"""Задача 4 (бонус) — provider-agnostic адаптер.

Досега всеки доставчик имаше свой начин да се извика и свои имена на
полетата. Тук ги скриваме зад една функция `ask(prompt, provider)` и един
общ тип `Answer`. Останалата част от програмата повече не знае — и не
трябва да знае — кой доставчик работи.

Това е първата стъпка към архитектура без vendor lock-in и е лайтмотивът на
курса. Framework-ите (LangChain, Vercel AI SDK — Теми 10-12) правят същото,
само в пъти по-голям мащаб.

Двете `_ask_*` функции са готови — сравнете ги ред по ред и вижте къде
доставчиците се разминават. Вашата задача е само `ask()`, около 4 реда.

Стартиране:
    python starter/04_ask_provider.py --provider anthropic "Здравей!"
    python starter/04_ask_provider.py --provider ollama    "Здравей!"
"""

from __future__ import annotations

import argparse
import sys
from dataclasses import dataclass
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import common`
from common import MODELS, SYSTEM, anthropic_client, require_ollama


@dataclass
class Answer:
    """Единен резултат, еднакъв за всички доставчици."""

    text: str
    input_tokens: int
    output_tokens: int


def _ask_anthropic(prompt: str) -> Answer:
    """Готово. Забележете кои полета се преименуват към Answer."""
    message = anthropic_client().messages.create(
        model=MODELS["anthropic"],
        max_tokens=512,
        system=SYSTEM,  # при Anthropic system е отделен параметър
        messages=[{"role": "user", "content": prompt}],
    )
    return Answer(
        text=message.content[0].text,
        input_tokens=message.usage.input_tokens,  # <- различно име
        output_tokens=message.usage.output_tokens,  # <- различно име
    )


def _ask_ollama(prompt: str) -> Answer:
    """Готово. Същата форма на заявката, други имена на полетата с токени."""
    import ollama

    require_ollama()
    response = ollama.chat(
        model=MODELS["ollama"],
        messages=[
            {"role": "system", "content": SYSTEM},  # при Ollama system е съобщение
            {"role": "user", "content": prompt},
        ],
    )
    return Answer(
        text=response["message"]["content"],
        input_tokens=response["prompt_eval_count"] or 0,  # <- различно име
        output_tokens=response["eval_count"] or 0,  # <- различно име
    )


def _ask_openai(prompt: str) -> Answer:
    """ЗА ДОМАШНО. Реализирайте го, без да пипате нищо извън тази функция.

    Насоки:
        from openai import OpenAI
        resp = OpenAI().chat.completions.create(model=..., messages=[...])
        текст   -> resp.choices[0].message.content
        токени  -> resp.usage.prompt_tokens / resp.usage.completion_tokens
    При OpenAI `system` е ПЪРВОТО съобщение с role="system" (както при Ollama,
    за разлика от Anthropic). Нужен е OPENAI_API_KEY в .env.
    Документация: https://platform.openai.com/docs/guides/text
    """
    raise NotImplementedError(
        "_ask_openai е за домашно — вижте указанията в docstring-а на функцията."
    )


_DISPATCH = {
    "anthropic": _ask_anthropic,
    "ollama": _ask_ollama,
    "openai": _ask_openai,
}


def ask(prompt: str, provider: str) -> Answer:
    """Единен интерфейс над всички доставчици.

    TODO: намерете в `_DISPATCH` функцията за подадения `provider`, извикайте
    я с `prompt` и върнете резултата. При непознат доставчик хвърлете
    ValueError с разбираемо съобщение.
    """
    raise NotImplementedError("Попълнете ask() — вижте TODO-то по-горе.")


def main() -> None:
    parser = argparse.ArgumentParser(description="Provider-agnostic LLM извикване.")
    parser.add_argument("--provider", choices=list(_DISPATCH), default="anthropic")
    parser.add_argument("prompt", nargs="+", help="Въпросът към модела.")
    args = parser.parse_args()

    try:
        answer = ask(" ".join(args.prompt), args.provider)
    except NotImplementedError as exc:
        sys.exit(str(exc))
    except Exception as exc:
        sys.exit(f"Грешка ({args.provider}): {type(exc).__name__}: {exc}")

    print(answer.text)
    print(f"\n[{args.provider}: вход={answer.input_tokens} изход={answer.output_tokens}]")
    print(
        "\nmain() не знае кой доставчик е работил — вижда само Answer."
        "\nТочно това е смисълът на адаптера."
    )


if __name__ == "__main__":
    main()
