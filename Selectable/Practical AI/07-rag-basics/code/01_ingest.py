"""Задача 1: подготовка на данните (LOAD, SPLIT, EMBED, STORE).

Вашата част е split_text в starter/chunk.py. Този скрипт е готов: зарежда корпуса,
дели го на части, векторизира ги и ги записва в pgvector. Пуска се веднъж,
а не при всеки въпрос.

Въпроси:
  а) Колко части излизат при размер 40 и при 200 (--chunk-size 200)? Защо?
  б) Пуснете с --overlap 0 и вижте как свършва една част и как започва
     следващата. Какво може да се изгуби на границата?
  в) Какво печелим и какво губим с много малки части (--chunk-size 10)?
     (Отговорът ще проличи и в задача 2.)

Изисква: docker compose up -d и ollama pull bge-m3

Стартиране:
    python 01_ingest.py
    python 01_ingest.py --chunk-size 200
    python 01_ingest.py --solutions        # с готовите решения, за сверка
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

from chunk import chunk_documents, load_documents, split_text

from dotenv import load_dotenv
from rag import embed
from store import Store


def main() -> None:
    load_dotenv()
    parser = argparse.ArgumentParser(description="Индексиране на корпуса.")
    parser.add_argument("--chunk-size", type=int, default=40, help="думи в една част")
    parser.add_argument("--overlap", type=int, default=10, help="думи припокриване")
    parser.add_argument(
        "--solutions", action="store_true", help="ползвай solutions/ вместо starter/"
    )
    args = parser.parse_args()

    if split_text("едно две три", 2, 0) is None:
        sys.exit("Първо попълнете split_text в chunk.py.")

    # Припокриване >= размера значи, че всяка част започва само една дума след
    # предишната: стотици почти еднакви части. Затова тогава делим без припокриване.
    if args.overlap >= args.chunk_size:
        print(
            f"Припокриването ({args.overlap}) трябва да е по-малко от размера "
            f"({args.chunk_size}), затова ползвам 0."
        )
        args.overlap = 0

    docs = load_documents(str(HERE / "corpus"))  # LOAD
    if not docs:
        sys.exit(f"Не намерих документи в {HERE / 'corpus'}.")
    chunks = chunk_documents(docs, args.chunk_size, args.overlap)  # SPLIT
    print(
        f"{len(docs)} документа -> {len(chunks)} части "
        f"(размер {args.chunk_size}, припокриване {args.overlap})\n"
    )

    for c in chunks[:2]:
        print(f"--- {c.id} ({len(c.text.split())} думи)\n{c.text}\n")

    vectors = embed([c.text for c in chunks])  # EMBED
    store = Store()
    store.clear()  # индексираме наново, за да сменяме размера свободно
    store.upsert(chunks, vectors)  # STORE
    print(
        f"Записани {len(chunks)} части в pgvector (вектори с {len(vectors[0])} измерения)."
    )


if __name__ == "__main__":
    main()
