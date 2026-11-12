"""Демо (мост към Тема 6): защо линейното търсене не мащабира.

Не е задача

Мерим колко време отнема една заявка, сравнена с всеки от N вектора, при
нарастващо N. Времето расте линейно с N, затова при милиони документи е нужна
векторна база с приблизителен (ANN) индекс. Векторите са случайни: интересува ни
само скоростта, не смисълът, а така демото върви офлайн и бързо.

Стартиране (от папка code/):
    python demo_scan_timing.py
"""

import time

import numpy as np

# Единственото място с numpy, и то заради скоростта, не заради математиката:
# формулата е в embeddings_util.py с обикновен Python. На чист Python 500 000
# вектора биха отнели минути и бихме мерили самия Python, а не алгоритъма.

DIM = 1024  # колкото bge-m3


def normalized_random(n: int, d: int) -> np.ndarray:
    m = np.random.default_rng(0).standard_normal((n, d)).astype(np.float32)
    return m / np.linalg.norm(m, axis=1, keepdims=True)


def linear_search(query: np.ndarray, matrix: np.ndarray, k: int = 5):
    # нормализирани вектори => косинус = скаларно произведение за всички наведнъж
    scores = matrix @ query
    return np.argpartition(-scores, k)[:k]


def main() -> None:
    print(f"{'N вектора':>12} {'време/заявка':>14}")
    print("-" * 28)
    for n in (1_000, 10_000, 100_000, 500_000):
        matrix = normalized_random(n, DIM)
        query = matrix[0]
        start = time.perf_counter()
        for _ in range(5):
            linear_search(query, matrix)
        elapsed = (time.perf_counter() - start) / 5
        print(f"{n:>12,} {elapsed * 1000:>11.1f} ms")

    print("\nВремето расте линейно с N. При милиони вектори това е твърде бавно за")
    print("всяка заявка, затова идва векторната база с ANN индекс (Тема 6).")


if __name__ == "__main__":
    main()
