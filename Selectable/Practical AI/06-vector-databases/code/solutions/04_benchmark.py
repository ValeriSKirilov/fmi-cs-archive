"""Задача 4 (за напредналите) - измерваме компромисите от лекцията.

Преди това: python solutions/bench.py

    python solutions/04_benchmark.py a    # 4а: точно срещу HNSW, ef_search
    python solutions/04_benchmark.py b    # 4б: IVFFlat срещу HNSW, probes
    python solutions/04_benchmark.py c    # 4в: филтър върху ANN индекс

recall@K = |точни ∩ приближени| / K - какъв дял от истинските K най-близки е
намерил индексът. Точният отговор получаваме, като изключим индекса: тогава
Postgres чете всички редове.

4г няма решение: това са отворени експерименти (виж README).
"""

import sys
import time

import numpy as np

from db import connect

K = 10
N_QUERIES = 20
CATEGORY = "c07"   # 1% от редовете

TOPK = "SELECT id FROM bench {where} ORDER BY embedding <=> %(q)s LIMIT %(k)s"


def run(cur, q, category=None) -> tuple[list[int], float]:
    """Една top-K заявка. Връща (ids, милисекунди)."""
    where = "WHERE category = %(cat)s" if category else ""
    t = time.perf_counter()
    cur.execute(TOPK.format(where=where), {"q": q, "k": K, "cat": category})
    ids = [row[0] for row in cur.fetchall()]
    return ids, (time.perf_counter() - t) * 1000


def exact(cur, q, category=None) -> tuple[list[int], float]:
    """Точно търсене: без индекс Postgres сканира всички редове."""
    cur.execute("SET enable_indexscan = off")
    try:
        return run(cur, q, category)
    finally:
        cur.execute("RESET enable_indexscan")


def recall(truth: list[int], found: list[int]) -> float:
    return len(set(truth) & set(found)) / len(truth)


def measure(cur, queries, truths, category=None) -> tuple[float, float, float]:
    """Среден recall, средно време (ms) и среден брой върнати редове."""
    results = [run(cur, q, category) for q in queries]
    rec = np.mean([recall(t, ids) for t, (ids, _ms) in zip(truths, results)])
    ms = np.mean([ms for _ids, ms in results])
    rows = np.mean([len(ids) for ids, _ms in results])
    return rec, ms, rows


def load_queries(cur) -> list[np.ndarray]:
    """Заявките, които bench.py е генерирал от същото разпределение като данните."""
    cur.execute("SELECT embedding FROM bench_queries ORDER BY id LIMIT %s", (N_QUERIES,))
    return [row[0] for row in cur.fetchall()]


def timed_ddl(cur, sql: str, index: str) -> str:
    t = time.perf_counter()
    cur.execute(sql)
    cur.execute("SELECT pg_size_pretty(pg_relation_size(%s))", (index,))
    return f"{time.perf_counter() - t:5.1f} s, {cur.fetchone()[0]}"


def part_a(cur, queries) -> None:
    truths, times = zip(*(exact(cur, q) for q in queries))
    print(f"точно (без индекс)     recall@{K} = 1.00   {np.mean(times):6.1f} ms")
    for ef in (10, 20, 40, 80, 200):
        cur.execute(f"SET hnsw.ef_search = {ef}")
        rec, ms, _rows = measure(cur, queries, truths)
        print(f"HNSW ef_search={ef:<4}   recall@{K} = {rec:.2f}   {ms:6.1f} ms")


def part_b(cur, queries) -> None:
    truths = [exact(cur, q)[0] for q in queries]
    n = cur.execute("SELECT count(*) FROM bench").fetchone()[0]
    lists = max(n // 1000, 10)   # препоръката на pgvector до ~1M реда

    # Два индекса върху една колона си пречат - сменяме ги.
    cur.execute("DROP INDEX IF EXISTS bench_hnsw")
    print(f"IVFFlat (lists={lists}) построен за",
          timed_ddl(cur, f"CREATE INDEX bench_ivf ON bench USING ivfflat "
                         f"(embedding vector_cosine_ops) WITH (lists = {lists})", "bench_ivf"))
    for probes in (1, 2, 5, 10, 20, lists):
        cur.execute(f"SET ivfflat.probes = {probes}")
        rec, ms, _rows = measure(cur, queries, truths)
        print(f"  probes={probes:<4}  recall@{K} = {rec:.2f}   {ms:6.1f} ms")

    cur.execute("DROP INDEX bench_ivf")
    print("HNSW построен за      ",
          timed_ddl(cur, "CREATE INDEX bench_hnsw ON bench USING hnsw "
                         "(embedding vector_cosine_ops)", "bench_hnsw"))
    rec, ms, _rows = measure(cur, queries, truths)
    print(f"  ef_search=40  recall@{K} = {rec:.2f}   {ms:6.1f} ms")


def part_c(cur, queries) -> None:
    truths = [exact(cur, q, CATEGORY)[0] for q in queries]
    print(f"WHERE category = '{CATEGORY}' (1% от редовете), искаме {K} реда")
    for mode in ("off", "relaxed_order", "strict_order"):
        cur.execute(f"SET hnsw.iterative_scan = {mode}")
        rec, ms, rows = measure(cur, queries, truths, CATEGORY)
        print(f"  iterative_scan={mode:<14} редове = {rows:4.1f}   recall@{K} = {rec:.2f}   {ms:6.1f} ms")


def main() -> None:
    part = sys.argv[1] if len(sys.argv) > 1 else ""
    parts = {"a": part_a, "b": part_b, "c": part_c}
    if part not in parts:
        sys.exit("Употреба: python solutions/04_benchmark.py a|b|c")

    with connect() as conn:
        cur = conn.cursor()
        if part == "c" and cur.execute("SELECT to_regclass('bench_hnsw')").fetchone()[0] is None:
            sys.exit("Няма индекс bench_hnsw: довършете стъпка 6 в 4б или пуснете отново bench.py.")
        parts[part](cur, load_queries(cur))
        conn.commit()


if __name__ == "__main__":
    main()
