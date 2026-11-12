"""Задача 3 (бонус): tool calling като structured output.

Задачи 1 и 2 минаха през `llm.chat(schema=...)`, без да ви интересува кой
доставчик отговаря. Тук отваряме капака: при Anthropic няма параметър „дай ми
JSON по тази схема“. Вместо това описваме **инструмент**, чийто вход Е
схемата, и принуждаваме модела да го извика:

    tools=[{"name": "record", "input_schema": <JSON схема>}]
    tool_choice={"type": "tool", "name": "record"}

Отговорът идва в `tool_use` блок, а `block.input` е вашият структуриран
резултат. Tool calling е по-общият механизъм; structured output е негов
частен случай (пълно разглеждане в Тема 9).

Изисква `ANTHROPIC_API_KEY` в `.env`. Без ключ пуснете само локалната част.

Стартиране:
    python solutions/03_providers.py                  # и двата пътя
    python solutions/03_providers.py --local-only     # без ключ
"""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import llm`
from llm import chat, cli, model_for

sys.path.insert(0, str(Path(__file__).resolve().parent))  # за `import schemas`
from schemas import Contact

SYSTEM = "Извличай структурирани данни от текста."
TEXT = "Иван Петров, системен администратор във ФМИ, пишете му на ivan@fmi.example.com"


def extract_local(text: str, args) -> Contact:
    """Дадено. През адаптера: не знаем и не ни интересува как е реализирано."""
    reply = chat(SYSTEM, [{"role": "user", "content": text}], schema=Contact,
                 temperature=0, provider="ollama", model=args.model)
    return Contact.model_validate_json(reply.text)


def extract_anthropic(text: str) -> Contact:
    """Вашето. Същото извличане, но написано на ръка през tool calling."""
    import anthropic

    message = anthropic.Anthropic().messages.create(
        model=model_for("anthropic"),
        max_tokens=512,
        system=SYSTEM,
        messages=[{"role": "user", "content": text}],
        tools=[{
            "name": "record",
            "description": "Запиши извлечения контакт.",
            "input_schema": Contact.model_json_schema(),  # схемата Е входът на инструмента
        }],
        tool_choice={"type": "tool", "name": "record"},  # принуда да го ползва
    )
    block = next(b for b in message.content if b.type == "tool_use")
    return Contact(**block.input)


def main() -> None:
    parser = cli("Tool calling като structured output.")
    parser.add_argument("--local-only", action="store_true", help="без Anthropic")
    args = parser.parse_args()

    if not Contact.model_fields:
        sys.exit("`Contact` няма полета. Първо попълнете schemas.py (Задача 1).")


    print(f"Текст: {TEXT}\n")
    print("--- локално, през адаптера (llm.chat(schema=...)) ---")
    print(f"   {extract_local(TEXT, args).model_dump()}")

    if args.local_only:
        return

    print("\n--- Anthropic, на ръка през tool calling ---")
    print(f"   {extract_anthropic(TEXT).model_dump()}")
    print("\nЕдин и същ резултат, два съвсем различни механизма отдолу."
          "\nЗатова разликите живеят в llm.py, а не из задачите. Това е същият"
          "\nадаптер от Упражнение 1, само с един параметър повече.")


if __name__ == "__main__":
    main()
