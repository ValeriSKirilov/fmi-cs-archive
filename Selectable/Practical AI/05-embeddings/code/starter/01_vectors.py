"""Задача 1: от текст до сходство.

Кодът тук е готов. Вашата част е в embeddings_util.py: dot, norm и cos_sim.
След това пуснете скрипта и отговорете на въпросите по-долу.

Скриптът показва четири неща:
  1) размерността на вектора е свойство на МОДЕЛА, а не на текста;
  2) векторите са нормализирани (||v|| = 1), затова cos_sim съвпада със
     скаларното произведение;
  3) един и същ текст дава един и същ вектор, затова векторите могат да се
     кешират и индексират веднъж;
  4) капанът: изречение и неговото отрицание излизат почти еднакви.

Въпроси:
  а) Зависи ли размерността от дължината на текста? Защо?
  б) Защо cos_sim и dot дават едно и също число тук? Кога не биха?
  в) Кое сходство е по-високо: на парафразата или на отрицанието?
     Какво означава това за търсенето?
  г) Какво следва от детерминизма за кеширането?

Изисква: ollama pull bge-m3

Стартиране:
    python starter/01_vectors.py
"""

import sys

from embeddings_util import OLLAMA_MODEL, cos_sim, dot, embed, norm

TEXTS = [
    "куче",
    "кученце",
    "автомобил",
    "данъчна декларация за 2025 г., подадена в срок и приета без забележки",
]

# Две теми, всяка казана по два начина без нито една обща дума.
PAIRS = [
    "Котката спи на дивана.",
    "Малкото коте дреме на канапето.",   # [1] парафраза на [0]
    "Автомобилът е паркиран пред блока.",
    "Колата стои отпред на улицата.",    # [3] парафраза на [2]
]

# Отрицанието мерим отделно: в общата матрица то би размило групите.
NEGATION = [
    "Влакът пристигна навреме.",
    "Влакът не пристигна навреме.",
]


def check_util() -> None:
    """Ясно съобщение, ако embeddings_util.py още не е попълнен."""
    if cos_sim([1.0, 0.0], [1.0, 0.0]) is None or norm([3.0, 4.0]) is None:
        sys.exit("Първо попълнете dot, norm и cos_sim в embeddings_util.py.")


def vectors() -> None:
    print(f"1) Какво е вектор (модел: {OLLAMA_MODEL})")
    vecs = embed(TEXTS)  # една заявка за всички текстове
    for text, v in zip(TEXTS, vecs):
        preview = text if len(text) <= 30 else text[:27] + "..."
        print(f"   d={len(v):<5} ||v||={norm(v):.4f}   {preview}")

    again = embed([TEXTS[0]])[0]
    delta = max(abs(x - y) for x, y in zip(again, vecs[0]))
    print(f"   същият текст, втора заявка: най-голяма разлика = {delta:.2e}\n")


def similarity() -> None:
    vecs = embed(PAIRS)

    cs, dp = cos_sim(vecs[0], vecs[1]), dot(vecs[0], vecs[1])
    print("2) cos_sim срещу скаларно произведение")
    print(f"   cos_sim = {cs:.4f}   dot = {dp:.4f}\n")

    print("3) Матрица на сходствата")
    print("       " + "  ".join(f"{f'[{i}]':>5}" for i in range(len(PAIRS))))
    for i, vi in enumerate(vecs):
        row = "  ".join(f"{cos_sim(vi, vj):5.2f}" for vj in vecs)
        print(f"   [{i}]  {row}   {PAIRS[i]}")

    print("   Най-близкото до всяко изречение:")
    for i in range(len(PAIRS)):
        j = max((k for k in range(len(PAIRS)) if k != i),
                key=lambda k: cos_sim(vecs[i], vecs[k]))
        mark = "двойката му" if j == i ^ 1 else "друго (!)"
        print(f"   [{i}] -> [{j}]  {cos_sim(vecs[i], vecs[j]):.3f}  {mark}")

    nv = embed(NEGATION)
    neg = cos_sim(nv[0], nv[1])
    weakest = min(cos_sim(vecs[0], vecs[1]), cos_sim(vecs[2], vecs[3]))
    print("\n4) Капанът с отрицанието")
    print(f"   {NEGATION[0]!r} срещу {NEGATION[1]!r}")
    print(f"   cos_sim = {neg:.3f}, а най-слабата истинска парафраза е {weakest:.3f}")


def main() -> None:
    check_util()
    vectors()
    similarity()


if __name__ == "__main__":
    main()
