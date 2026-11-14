"""Общи помощни функции за embeddings.

По подразбиране ползваме ЛОКАЛЕН модел. Математиката е една и съща, независимо
кой модел създава векторите, а тя е същината на темата. Облачен вариант (OpenAI)
има през EMBED_PROVIDER=openai, но нито една задача не се нуждае от него: стои
само за сравнение.

Косинусовото сходство без библиотеки

    cos(a, b) = (a · b) / (||a|| * ||b||)

Един вектор тук е просто списък от числа. Скаларното произведение е сума от
произведенията по координати; дължината е корен от сумата на квадратите
(Питагор, само в 1024 измерения вместо в 2).
"""

import math
import os

EMBED_PROVIDER = os.environ.get("EMBED_PROVIDER", "ollama")

# bge-m3 е МНОГОЕЗИЧЕН и затова е по подразбиране. Популярните английски модели
# (nomic-embed-text, all-minilm) се справят зле с български: при търсене връщат
# почти случайни резултати. Задача 3 го показва на живо.
OLLAMA_MODEL = "bge-m3"                    # 1024 измерения; ollama pull bge-m3
OPENAI_MODEL = "text-embedding-3-small"    # 1536 измерения; изисква ключ


def dot(a, b) -> float:
    """Скаларно произведение: a·b = a[0]*b[0] + a[1]*b[1] + ... + a[d-1]*b[d-1]."""
    return sum(x * y for x, y in zip(a, b))


def norm(v) -> float:
    """Дължина на вектора: ||v|| = sqrt(v[0]^2 + v[1]^2 + ...), т.е. Питагор в d измерения."""
    return math.sqrt(sum(x * x for x in v))


def cos_sim(a, b) -> float:
    """Косинусово сходство = (a · b) / (||a|| * ||b||), направо по дефиницията.

    Делението на дължините прави мярката независима от „големината“ на вектора:
    важна е ПОСОКАТА. Затова при нормализирани вектори (||v|| = 1) знаменателят
    е 1 и cos_sim се свежда до самото скаларно произведение.
    """
    return dot(a, b) / (norm(a) * norm(b))


def embed(texts: list[str], model: str | None = None) -> list[list[float]]:
    """Векторизира всички текстове наведнъж (batch), с една заявка.

    Разликата е съществена. При облачен модел пращаме една HTTP заявка вместо N,
    а локалният модел се извиква веднъж, а не N пъти. Затова е добре текстовете
    да се векторизират на групи, а не един по един.
    """
    if EMBED_PROVIDER == "openai":
        # Референтен вариант (не е нужен за задачите)
        from openai import OpenAI

        resp = OpenAI().embeddings.create(model=model or OPENAI_MODEL, input=texts)
        return [d.embedding for d in resp.data]

    import ollama

    return ollama.embed(model=model or OLLAMA_MODEL, input=texts)["embeddings"]


def dim() -> int:
    """Размерност на текущия модел (свойство на модела, не на текста)."""
    return 1536 if EMBED_PROVIDER == "openai" else 1024
