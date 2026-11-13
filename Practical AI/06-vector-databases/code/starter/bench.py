"""Данни за задача 4 - дадено, пуска се веднъж.

Създава ОТДЕЛНИ таблици (docs от задачи 1-3 не се пипа):
    bench          N синтетични вектора + HNSW индекс върху тях
    bench_queries  20 заявки от същото разпределение

24 документа са твърде малко: при тях индексът няма значение и ANN винаги
намира точния отговор.

Векторите не са от модел, а синтетични: 100 случайни центъра и всеки вектор е
център + шум. Така приличат на истински embeddings, които също образуват групи
по теми. (Напълно случайни вектори са най-лошият случай за ANN - опитайте в 4г.)

Защо 128 измерения, а не 1024: при синтетични вектори размерността не носи
смисъл, а с по-малко измерения генерирането и индексът са по-бързи.

Категориите c00 … c99 са по 1% от редовете и не зависят от групата - нужни са
за частта с филтъра (4в).

Стартиране:
    python starter/bench.py            # 50 000 реда
    python starter/bench.py 200000     # или колкото искате (4г)
"""

import sys
import time

from db import connect

DIM = 128
CLUSTERS = 100
NOISE = 2.0  # по-голям шум = по-размити групи = по-труден ANN (4г)

# 100 центъра. Подзаявката споменава c (WHERE c >= 0), за да се изчислява
# отново за всеки ред - иначе всички центрове биха били еднакви.
CENTERS = f"""
CREATE TEMP TABLE centers AS
SELECT c, (SELECT array_agg(random() - 0.5) FROM generate_series(1, {DIM}) WHERE c >= 0) AS v
  FROM generate_series(0, {CLUSTERS - 1}) AS c
"""

# Вектор = центърът на групата + шум. Групата е mod(i, 97), а категорията
# mod(i, 100), така че двете са независими.
POINTS = f"""
SELECT i, 'c' || lpad(mod(i, 100)::text, 2, '0'),
       (SELECT array_agg(ce.v[j] + {NOISE} * (random() - 0.5))::real[]::vector({DIM})
          FROM generate_series(1, {DIM}) AS j)
  FROM generate_series(1, %s) AS i
  JOIN centers AS ce ON ce.c = mod(i * 7919, {CLUSTERS})
"""


def main() -> None:
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 50_000
    with connect() as conn:
        conn.execute("DROP TABLE IF EXISTS bench, bench_queries")
        conn.execute(
            f"CREATE TABLE bench (id int PRIMARY KEY, category text, embedding vector({DIM}))"
        )
        conn.execute(
            f"CREATE TABLE bench_queries (id int PRIMARY KEY, category text, embedding vector({DIM}))"
        )

        t = time.perf_counter()
        conn.execute("SELECT setseed(0.42)")
        conn.execute(CENTERS)
        conn.execute("INSERT INTO bench " + POINTS, (n,))
        conn.execute(
            "INSERT INTO bench_queries "
            + POINTS.replace("i * 7919", "(i + 13) * 7919"),
            (20,),
        )
        conn.execute("ANALYZE bench")
        print(f"{n} реда за {time.perf_counter() - t:.1f} s")

        t = time.perf_counter()
        conn.execute(
            "CREATE INDEX bench_hnsw ON bench USING hnsw (embedding vector_cosine_ops)"
        )
        size = conn.execute(
            "SELECT pg_size_pretty(pg_relation_size('bench_hnsw'))"
        ).fetchone()[0]
        print(f"HNSW индекс за {time.perf_counter() - t:.1f} s, размер {size}")
        conn.commit()


if __name__ == "__main__":
    main()
