"""Задача 1 - зареждане (upsert): текст → вектор → ред в таблицата.

Един запис във векторната база е: id + вектор + метаданни (+ самият текст).
Тук това е буквално един ред в таблицата docs.

ON CONFLICT ... DO UPDATE е upsert: ако id вече съществува, редът се обновява.
Затова скриптът може да се пуска многократно, без да се дублират записи.

Стартиране:
    python starter/01_ingest.py
"""

from corpus import DOCS
from db import connect, embed

# TODO: довършете заявката.
#   1. Колоните са в реда, в който са стойностите в rows по-долу (вижте
#      таблицата в db/init.sql). За всяка колона: един %s във VALUES.
#   2. Направете я upsert: ако ред със същия id вече съществува, обновете
#      останалите му колони, вместо да гърми с duplicate key.
#      Синтаксис в PostgreSQL:
#        ON CONFLICT (<ключ>) DO UPDATE SET <колона> = EXCLUDED.<колона>, ...
#      EXCLUDED е редът, който се опитахме да вмъкнем.
UPSERT = """
INSERT INTO docs (id, content, category, year, embedding)
VALUES (%s, %s, %s, %s, %s) 
ON CONFLICT (id) DO UPDATE 
    SET content = EXCLUDED.content, category = EXCLUDED.category, year = EXCLUDED.year, embedding = EXCLUDED.embedding
"""


def main() -> None:
    texts = [text for _id, text, _cat, _year in DOCS]
    vectors = embed(texts)
    print(f"{len(vectors)} вектора по {len(vectors[0])} измерения")

    rows = [
        (i, text, cat, year, vec) for (i, text, cat, year), vec in zip(DOCS, vectors)
    ]

    with connect() as conn:
        conn.cursor().executemany(UPSERT, rows)
        conn.commit()
        count = conn.execute("SELECT count(*) FROM docs").fetchone()[0]
        print(f"Редове в таблицата docs: {count}")


if __name__ == "__main__":
    main()
