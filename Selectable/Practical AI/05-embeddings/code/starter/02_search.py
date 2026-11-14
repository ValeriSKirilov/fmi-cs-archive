"""Задачи 2 и 3: семантично търсене срещу BM25 и изборът на модел.

Задача 2. Попълнете search(): векторизирайте заявката и върнете top-k
документа по косинусово сходство. BM25 е готов: идеята му е в лекцията
(честота на думата, IDF и нормализация по дължина), а кодът е долу.
Пуснете скрипта и обяснете кой подход печели при всяка от двете заявки и защо.

Задача 3. Пуснете същия скрипт с английския модел:
    python starter/02_search.py --model all-minilm
и сравнете колко от намерените документи са верни (отбелязани с +).
Въпроси: английският модел „греши“ ли? Може ли да сравним вектор от bge-m3 с
вектор от all-minilm? Какво трябва да направим, ако сменим модела?

Бонус: пробвайте заявка на английски (--query "vehicle") с bge-m3.

Изисква: ollama pull bge-m3 (и ollama pull all-minilm за задача 3)

Стартиране:
    python starter/02_search.py
    python starter/02_search.py --model all-minilm
    python starter/02_search.py --query "vehicle"
"""

import argparse
import math
import re
import sys
from collections import Counter

from corpus import DOCS
from dotenv import load_dotenv
from embeddings_util import OLLAMA_MODEL, cos_sim, embed

QUERIES = ["превозно средство", "SKU-4471"]

# Кои документи са верни за всяка заявка, за да броим попаденията, а не „на око“.
RELEVANT = {
    "превозно средство": {
        "Автомобилът има нов двигател и харчи малко гориво.",
        "Колата е паркирана на улицата пред блока.",
        "Влакът за София тръгва в 8 часа от централна гара.",
        "Самолетът кацна с половин час закъснение.",
    },
    "SKU-4471": {"Артикул с код SKU-4471 е изчерпан от склада."},
}
RELEVANT["vehicle"] = RELEVANT["превозно средство"]  # за бонуса


def search(query: str, doc_vecs: list[list[float]], model: str, k: int = 3):
    """Семантично търсене: top-k документа по косинусово сходство."""
    q = embed([query], model=model)[0]
    scored = sorted(
        ((cos_sim(q, v), doc) for doc, v in zip(DOCS, doc_vecs)), reverse=True
    )
    return scored[:k]

    # 1) векторизирайте заявката: q = embed([query], model=model)[0]
    # 2) за всеки документ сметнете (cos_sim(q, v), документ);
    #       zip(DOCS, doc_vecs) ви дава двойките документ и вектор
    # 3) сортирайте намаляващо и върнете първите k


# --- BM25 (готов) ------------------------------------------------------------
def tokenize(text: str) -> list[str]:
    return re.findall(r"\w+", text.lower())


class BM25:
    """Класическото пълнотекстово търсене: съвпадение по думи.

    Описан е в Robertson и Zaragoza (2009), вж. библиографията на лекцията.
    """

    def __init__(self, docs: list[str], k1: float = 1.5, b: float = 0.75):
        self.raw = docs
        self.docs = [tokenize(d) for d in docs]
        self.k1, self.b = k1, b
        self.avgdl = sum(len(d) for d in self.docs) / len(self.docs)
        self.df = Counter(t for d in self.docs for t in set(d))
        self.N = len(self.docs)

    def idf(self, term: str) -> float:
        n = self.df.get(term, 0)
        return math.log(1 + (self.N - n + 0.5) / (n + 0.5))

    def score(self, query: str, i: int) -> float:
        doc = self.docs[i]
        freqs = Counter(doc)
        s = 0.0
        for term in tokenize(query):
            f = freqs.get(term, 0)
            if f:
                denom = f + self.k1 * (1 - self.b + self.b * len(doc) / self.avgdl)
                s += self.idf(term) * f * (self.k1 + 1) / denom
        return s

    def search(self, query: str, k: int = 3):
        scored = sorted(
            ((self.score(query, i), self.raw[i]) for i in range(self.N)), reverse=True
        )
        return scored[:k]


# --- изход ---------------------------------------------------------------------
def show(title: str, results, relevant: set[str]) -> None:
    hits = sum(doc in relevant for _, doc in results)
    print(f"  {title}" + (f"  (верни: {hits}/{len(results)})" if relevant else ""))
    for score, doc in results:
        mark = "+" if doc in relevant else " "
        print(f"    {mark} {score:6.3f}  {doc}")


def main() -> None:
    load_dotenv()
    parser = argparse.ArgumentParser(description="Семантично търсене срещу BM25.")
    parser.add_argument(
        "--model",
        default=OLLAMA_MODEL,
        help="embedding модел (за задача 3: all-minilm)",
    )
    parser.add_argument("--query", nargs="+", help="собствена заявка")
    args = parser.parse_args()

    queries = [" ".join(args.query)] if args.query else QUERIES
    doc_vecs = embed(DOCS, model=args.model)  # векторизираме корпуса веднъж
    print(f"Модел: {args.model}  (d = {len(doc_vecs[0])})")

    bm25 = BM25(DOCS)
    for query in queries:
        results = search(query, doc_vecs, args.model)
        if results is None:
            sys.exit("Първо попълнете search().")
        relevant = RELEVANT.get(query, set())
        print(f"\nЗаявка: {query!r}")
        show("BM25:", bm25.search(query), relevant)
        show("семантично:", results, relevant)


if __name__ == "__main__":
    main()
