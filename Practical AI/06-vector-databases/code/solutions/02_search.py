"""Задачи 2 и 3 - търсене (query) и филтър по метаданни.

Задача 2: най-близките K документа до заявката. Операторът <=> връща
КОСИНУСОВО РАЗСТОЯНИЕ: по-малко = по-близо, затова подреждаме възходящо.
Сходството е 1 - разстоянието.

Задача 3: филтърът по метаданни в pgvector е обикновен SQL WHERE.

Стартиране:
    python solutions/02_search.py "превозно средство"
    python solutions/02_search.py "превозно средство" --category transport
"""

import argparse

from db import connect, embed

SQL = """
SELECT id, category, content, embedding <=> %(q)s AS dist
  FROM docs
  {where}
 ORDER BY embedding <=> %(q)s
 LIMIT %(k)s
"""


def search(query: str, k: int = 5, category: str | None = None) -> list[tuple]:
    """Връща до k реда (id, category, content, dist), най-близките първи."""
    qvec = embed([query])[0]
    where = "WHERE category = %(cat)s" if category else ""
    with connect() as conn:
        return conn.execute(SQL.format(where=where), {"q": qvec, "k": k, "cat": category}).fetchall()


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("query", nargs="?", default="превозно средство")
    parser.add_argument("--category", help="филтър по метаданни, напр. transport")
    parser.add_argument("-k", type=int, default=5)
    args = parser.parse_args()

    print(f"Заявка: {args.query!r}  категория: {args.category or 'всички'}\n")
    for _id, cat, content, dist in search(args.query, args.k, args.category):
        print(f"  разстояние={dist:.3f}  сходство={1 - dist:.3f}  [{cat}] {content}")


if __name__ == "__main__":
    main()
