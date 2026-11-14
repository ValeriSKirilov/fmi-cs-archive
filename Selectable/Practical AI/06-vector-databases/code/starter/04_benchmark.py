"""Задача 4 (за напредналите) - измерваме компромисите от лекцията.

Преди това: python starter/bench.py

    python starter/04_benchmark.py a    # 4а: точно срещу HNSW, ef_search
    python starter/04_benchmark.py b    # 4б: IVFFlat срещу HNSW, probes
    python starter/04_benchmark.py c    # 4в: филтър върху ANN индекс

recall@K = |точни ∩ приближени| / K - какъв дял от истинските K най-близки е
намерил индексът. Точният отговор получаваме, като изключим индекса: тогава
Postgres чете всички редове.

Свършихте a, b и c? Продължете с 4г в README - там няма край.
"""

import sys
import time

import numpy as np
from db import connect

K = 10
N_QUERIES = 20
CATEGORY = "c07"  # 1% от редовете

TOPK = "SELECT id FROM bench {where} ORDER BY embedding <=> %(q)s LIMIT %(k)s"


# ---------------------------------------------------------------- дадено ---


def run(cur, q, category=None) -> tuple[list[int], float]:
    """Една top-K заявка през каквото Postgres избере. Връща (ids, милисекунди)."""
    where = "WHERE category = %(cat)s" if category else ""
    t = time.perf_counter()
    cur.execute(TOPK.format(where=where), {"q": q, "k": K, "cat": category})
    ids = [row[0] for row in cur.fetchall()]
    return ids, (time.perf_counter() - t) * 1000


def load_queries(cur) -> list[np.ndarray]:
    """Заявките, които bench.py е генерирал от същото разпределение като данните."""
    cur.execute(
        "SELECT embedding FROM bench_queries ORDER BY id LIMIT %s", (N_QUERIES,)
    )
    return [row[0] for row in cur.fetchall()]


def timed_ddl(cur, sql: str, index: str) -> str:
    """Изпълнява CREATE INDEX и връща „време, размер на индекса“."""
    t = time.perf_counter()
    cur.execute(sql)
    cur.execute("SELECT pg_size_pretty(pg_relation_size(%s))", (index,))
    return f"{time.perf_counter() - t:5.1f} s, {cur.fetchone()[0]}"


# ------------------------------------------------------------- вашият код ---


def exact(cur, q, category=None) -> tuple[list[int], float]:
    """Точно търсене (brute force)."""
    # TODO: изключете индекса със SET enable_indexscan = off, извикайте run(),
    #       и ВИНАГИ върнете настройката обратно (RESET enable_indexscan) -
    #       подсказка: try/finally.
    raise NotImplementedError


def recall(truth: list[int], found: list[int]) -> float:
    # TODO: размерът на сечението на двете множества, делено на len(truth).
    raise NotImplementedError


def measure(cur, queries, truths, category=None) -> tuple[float, float, float]:
    """Среден recall, средно време (ms) и среден брой върнати редове."""
    # TODO: пуснете run() за всяка заявка и усреднете трите величини.
    raise NotImplementedError


def part_a(cur, queries) -> None:
    # TODO 4а:
    #   1. Точните отговори (truths) и средното им време с exact().
    #   2. За ef в (10, 20, 40, 80, 200): SET hnsw.ef_search = <ef>, после measure().
    #      Внимание: SET не приема %s параметри (дава SyntaxError at "$1").
    #      Сглобете заявката с f-string: числото е ваше, не идва от потребител.
    #   3. Отпечатайте по ред на стойност: ef_search, recall@K, ms.
    #   Въпрос: коя стойност бихте избрали за продукт и защо?
    raise NotImplementedError


def part_b(cur, queries) -> None:
    # TODO 4б:
    #   1. Точните отговори, както в 4а.
    #   2. lists = брой редове // 1000 (препоръката на pgvector до ~1M реда).
    #   3. DROP INDEX bench_hnsw - два индекса върху една колона си пречат.
    #   4. С timed_ddl() постройте IVFFlat:
    #        CREATE INDEX bench_ivf ON bench
    #          USING ivfflat (embedding vector_cosine_ops) WITH (lists = ...)
    #   5. За probes в (1, 2, 5, 10, 20, lists): SET ivfflat.probes (с f-string), measure().
    #   6. DROP INDEX bench_ivf и постройте отново HNSW (пак с timed_ddl).
    #      Задължително: без него 4в ще мери без индекс.
    #      Сравнете: време за строене, размер, recall при подобна латентност.
    #   Въпрос: какво точно прави probes = lists и защо recall става 1.00?
    raise NotImplementedError


def part_c(cur, queries) -> None:
    # TODO 4в: филтърът от слайда „pgvector: филтър заедно с HNSW индекс“.
    #   1. Точните отговори С ФИЛТЪРА: exact(cur, q, CATEGORY).
    #   2. За mode в ("off", "relaxed_order", "strict_order"):
    #        SET hnsw.iterative_scan = <mode> (с f-string), measure(..., CATEGORY).
    #   3. Отпечатайте средния БРОЙ върнати редове, recall и ms.
    #   Въпроси: защо при "off" редовете са почти 0, а поискахме 10?
    #            кой режим дава по-висок recall? Прочетете в README на pgvector
    #            какво гарантира всеки от двата режима.
    raise NotImplementedError


def main() -> None:
    part = sys.argv[1] if len(sys.argv) > 1 else ""
    parts = {"a": part_a, "b": part_b, "c": part_c}
    if part not in parts:
        sys.exit("Употреба: python starter/04_benchmark.py a|b|c")

    with connect() as conn:
        cur = conn.cursor()
        if (
            part == "c"
            and cur.execute("SELECT to_regclass('bench_hnsw')").fetchone()[0] is None
        ):
            sys.exit(
                "Няма индекс bench_hnsw: довършете стъпка 6 в 4б или пуснете отново bench.py."
            )
        parts[part](cur, load_queries(cur))
        conn.commit()


if __name__ == "__main__":
    main()
