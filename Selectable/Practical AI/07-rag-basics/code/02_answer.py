"""Задача 2: отговор с източници (RETRIEVE, AUGMENT, GENERATE).

Вашата част е в starter/rag.py: GROUNDED_SYSTEM и build_prompt. Този скрипт е готов.
Без аргумент задава два въпроса от корпуса и два извън него.

Въпроси:
  а) Какво отговаря системата на въпросите извън корпуса?
  б) Пуснете с --no-grounding. Как се променят отговорите? Защо?
  в) Цитатите [1], [2] сочат ли наистина към частите, в които е отговорът?

Бонус: --stream (отговорът парче по парче, Тема 4) и --json (цитатите като
структуриран изход, Тема 3).

Стартиране (след 01_ingest.py):
    python 02_answer.py
    python 02_answer.py --no-grounding
    python 02_answer.py "Колко дни е годишният отпуск?" --stream
    python 02_answer.py "Какво означава грешка E-2001?" --json
    python 02_answer.py --solutions        # с готовите решения, за сверка
"""

import argparse
import sys
from pathlib import Path

# Вашият код (chunk.py, rag.py) е в starter/. С --solutions скриптът ползва
# готовите решения от solutions/, за сверка.
HERE = Path(__file__).parent
sys.path.insert(
    0, str(HERE / ("solutions" if "--solutions" in sys.argv else "starter"))
)

from dotenv import load_dotenv
from generate import generate, generate_json, stream
from pydantic import BaseModel
from rag import GROUNDED_SYSTEM, PLAIN_SYSTEM, build_prompt, retrieve
from store import Store

QUESTIONS = [
    "Колко дни предварително се заявява отпуск?",  # в корпуса
    "Какъв е лимитът за хотел при командировка в чужбина?",  # в корпуса
    "Каква е политиката за домашни любимци в офиса?",  # извън корпуса
    "Колко струва паркингът пред офиса?",  # извън корпуса
]


class Answer(BaseModel):
    answer: str
    citations: list[int]  # номерата [n] на използваните части


JSON_HINT = (
    " Върни JSON с полета 'answer' (текст) и 'citations' (номерата на "
    "използваните части). Ако няма информация, citations е празен списък."
)


def show_sources(hits) -> None:
    print("  Намерени части:")
    for i, h in enumerate(hits, start=1):
        print(f"    [{i}] {h.score:.3f}  {h.source} (част {h.chunk_index})")


def ask(store, question: str, args) -> None:
    hits = retrieve(store, question)
    prompt = build_prompt(question, hits)
    system = PLAIN_SYSTEM if args.no_grounding else GROUNDED_SYSTEM

    print(f"\nВъпрос: {question}")
    show_sources(hits)
    if args.json:
        result = generate_json(system + JSON_HINT, prompt, Answer)
        print(f"  Отговор: {result.answer}\n  Цитати: {result.citations}")
    elif args.stream:
        print("  Отговор: ", end="", flush=True)
        for delta in stream(system, prompt):
            print(delta, end="", flush=True)
        print()
    else:
        print(f"  Отговор: {generate(system, prompt)}")
        print()


def main() -> None:
    load_dotenv()
    parser = argparse.ArgumentParser(description="RAG отговор с източници.")
    parser.add_argument("question", nargs="*")
    parser.add_argument(
        "--no-grounding",
        action="store_true",
        help="без инструкцията „само от контекста“",
    )
    parser.add_argument("--stream", action="store_true", help="бонус: стрийминг")
    parser.add_argument("--json", action="store_true", help="бонус: цитати като JSON")
    parser.add_argument(
        "--solutions", action="store_true", help="ползвай solutions/ вместо starter/"
    )
    args = parser.parse_args()

    if not GROUNDED_SYSTEM or build_prompt("въпрос", []) is None:
        sys.exit("Първо попълнете GROUNDED_SYSTEM и build_prompt в rag.py.")

    store = Store()
    for q in [" ".join(args.question)] if args.question else QUESTIONS:
        ask(store, q, args)


if __name__ == "__main__":
    main()
