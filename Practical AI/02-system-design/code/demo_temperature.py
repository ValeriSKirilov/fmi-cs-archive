"""Демонстрация (пуска я преподавателят): API-то се смени под краката ни.

Наивният ред `client.messages.create(..., temperature=0)` днес се чупи по два
независими начина. Три опита, едно и също желание („temperature=0“):

  1) SDK 1.x махна temperature от сигнатурата    -> TypeError, още преди мрежата
  2) през extra_body към най-новия модел           -> HTTP 400 от самото API
  3) през extra_body към по-старо поколение        -> OK

Изводът: знанието „кой модел какво приема“ остарява. Затова то живее на
ЕДНО място (ANTHROPIC_SAMPLING в llm.py), а задачите не знаят за него.

Изисква ANTHROPIC_API_KEY в .env.

Стартиране:
    python demo_temperature.py
"""

from __future__ import annotations

import os
import sys

import llm  # зарежда .env

import anthropic

NEW_MODEL = "claude-sonnet-5-5"
OLD_MODEL = "claude-haiku-4-5"
MSG = [{"role": "user", "content": "Кажи „ок“."}]


def attempt(title: str, call) -> None:
    print(f"\n{title}")
    try:
        msg = call()
        text = next((b.text for b in msg.content if b.type == "text"), "")
        print(f"   OK -> {text.strip()!r}")
    except TypeError as exc:
        print(f"   TypeError -> {exc}")
    except anthropic.APIStatusError as exc:
        error = (exc.body or {}).get("error", {}) if isinstance(exc.body, dict) else {}
        print(f"   HTTP {exc.status_code} -> {error.get('message', exc)}")


def main() -> None:
    if not os.environ.get("ANTHROPIC_API_KEY"):
        sys.exit("Демото изисква ANTHROPIC_API_KEY в .env.")
    client = anthropic.Anthropic()

    attempt(f"1) temperature=0 като аргумент ({OLD_MODEL})",
            lambda: client.messages.create(model=OLD_MODEL, max_tokens=16,
                                           messages=MSG, temperature=0))
    attempt(f"2) extra_body temperature=0 ({NEW_MODEL})",
            lambda: client.messages.create(model=NEW_MODEL, max_tokens=16, messages=MSG,
                                           extra_body={"temperature": 0}))
    attempt(f"3) extra_body temperature=0 ({OLD_MODEL})",
            lambda: client.messages.create(model=OLD_MODEL, max_tokens=16, messages=MSG,
                                           extra_body={"temperature": 0}))

    print(f"\nВ llm.py: ANTHROPIC_SAMPLING = {sorted(llm.ANTHROPIC_SAMPLING)}"
          "\nЕдин ред за сменяне, когато платформата се промени пак.")


if __name__ == "__main__":
    main()
