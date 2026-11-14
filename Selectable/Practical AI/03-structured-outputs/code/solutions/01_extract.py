"""Задача 1: „върни JSON, моля“ срещу гарантирана схема.

Един и същ текст минава по два пътя:

  `extract_naive()`   дадено. Молим го с думи да върне JSON и парсим на ръка.
  `extract_schema()`  вашето. Подаваме СХЕМАТА и валидираме с Pydantic.

Разликата е един параметър. Но той не е инструкция към модела, а ОГРАНИЧЕНИЕ
при декодирането: при Ollama схемата става граматика в llama.cpp и моделът
физически не може да изведе токен, който я нарушава. Затова не може да добави
```json огради, обяснение отпред или запетая в повече, колкото и да „иска“.

Печалбата е в надеждността, не в скоростта: двата пътя отнемат сходно време.

Стартиране:
    python solutions/01_extract.py
    python solutions/01_extract.py --classify          # enum вместо извличане
    python solutions/01_extract.py --model llama3.2:1b

Изход (bggpt:4b):
    --- наивно: „върни JSON“ + json.loads ---
     1. ПРОВАЛ  json.loads: отговорът започва с '```json\n{\n  "name"'   1.9s
     ...
    Успешни: 0/4   общо време: 6.8s

    --- по схема: format=<JSON схема> + Pydantic ---
     1. OK      Иван Петров                                              1.6s
     ...
    Успешни: 4/4   общо време: 6.4s
"""

from __future__ import annotations

import json
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import llm`
from llm import chat, cli

sys.path.insert(0, str(Path(__file__).resolve().parent))  # за `import schemas`
from schemas import Contact, Review

SYSTEM = "Извличай структурирани данни от текста."
SYSTEM_NAIVE = SYSTEM + " Отговори само с JSON обект с полета name, email, company, role."
SYSTEM_TONE = "Класифицирай тона на отзива и обясни накратко защо."

TEXTS = [
    "Иван Петров, системен администратор във ФМИ, пишете му на ivan@fmi.example.com",
    "Мария Георгиева, счетоводител в Фирма АД, maria@firma.example.com",
    "Пишете на Георги Иванов (QA в BBS Apps) на g.ivanov@bbsapps.example.com",
    "д-р Елена Стоянова, лекар в болница, elena@hospital.example.com",
]

REVIEWS = [
    "Обслужването беше отвратително, чаках цял час.",
    "Най-добрата покупка за годината, препоръчвам!",
    "Пристигна във вторник, предлага се в три цвята.",
]


def extract_naive(text: str, args) -> str:
    """Дадено. Крехкият подход: молим за JSON с думи и парсим на ръка."""
    reply = chat(SYSTEM_NAIVE, [{"role": "user", "content": text}],
                 temperature=0, provider=args.provider, model=args.model)
    try:
        return json.loads(reply.text)["name"]  # ето тук се проваля
    except json.JSONDecodeError:
        raise ValueError(f"json.loads: отговорът започва с {reply.text[:18]!r}") from None


def extract_schema(text: str, args) -> str:
    """Вашето. Същата заявка, но със СХЕМА.

    Връща името от извлечения контакт.
    """
    reply = chat(SYSTEM, [{"role": "user", "content": text}],
                 schema=Contact,  # <- единствената разлика
                 temperature=0, provider=args.provider, model=args.model)
    contact = Contact.model_validate_json(reply.text)
    return contact.name


def classify(text: str, args) -> str:
    """Същият механизъм, друга схема: изброим тип вместо извличане."""
    reply = chat(SYSTEM_TONE, [{"role": "user", "content": text}],
                 schema=Review, temperature=0,
                 provider=args.provider, model=args.model)
    if reply.truncated:
        # Схемата не пази от отрязване: ако max_tokens свърши насред низа,
        # JSON-ът е невалиден въпреки граматиката.
        raise ValueError("отговорът е отрязан (max_tokens), JSON-ът е непълен")
    review = Review.model_validate_json(reply.text)
    return f"{review.tone.value}: {review.reason[:36]}"


def run(title: str, texts: list[str], fn, args) -> None:
    """Пуска `fn` върху всички текстове и мери времето еднакво за двата пътя."""
    print(f"\n--- {title} ---")
    ok = total = 0.0
    for i, text in enumerate(texts, start=1):
        start = time.perf_counter()
        try:
            value = fn(text, args)
            ok += 1
            status = f"OK      {value[:52]}"
        except Exception as exc:
            status = f"ПРОВАЛ  {exc}"
        seconds = time.perf_counter() - start
        total += seconds
        print(f" {i}. {status:<62} {seconds:>5.1f}s")
    print(f"Успешни: {int(ok)}/{len(texts)}   общо време: {total:.1f}s")


def main() -> None:
    parser = cli("Структуриран изход: наивно срещу по схема.")
    parser.add_argument("--classify", action="store_true",
                        help="enum класификация вместо извличане")
    args = parser.parse_args()

    if args.classify:
        run("класификация по enum схема", REVIEWS, classify, args)
        print("\nМоделът не може да върне дума извън трите в `Tone`:"
              "\nизброимият тип също е ограничение при декодирането."
              "\n\nНо вижте първия отзив: „обслужването беше отвратително“"
              "\nполучава етикет НЕУТРАЛНО, и то при всяко пускане. Enum-ът"
              "\nгарантира, че отговорът е една от трите думи, но не че е вярната."
              "\nСъщото разграничение валидно/вярно видяхте в eval-а от"
              "\nУпражнение 2.")
        return

    run("наивно: „върни JSON“ + json.loads", TEXTS, extract_naive, args)
    run("по схема: format=<JSON схема> + Pydantic", TEXTS, extract_schema, args)
    print("\nЕдно и също API, една и съща заявка. Разликата е един параметър,"
          "\nкойто не е молба към модела, а ограничение при декодирането."
          "\n\nТук наивният път се проваля все по един и същи начин и затова"
          "\nизглежда лесно поправим: махаме ```json оградите и продължаваме."
          "\nСамо че утре моделът ще сложи обяснение преди JSON-а, вдругиден"
          "\nзапетая в повече, после два обекта вместо един. Кърпенето няма"
          "\nкрай. Схемата не лекува симптом, а премахва целия клас проблеми.")


if __name__ == "__main__":
    main()
