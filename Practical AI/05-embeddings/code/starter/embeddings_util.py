"""Общи помощни функции за embeddings. Задача 1: попълнете трите TODO-та.

По подразбиране ползваме ЛОКАЛЕН многоезичен модел (Ollama): безплатно, офлайн
и без ключ. Облачен вариант (OpenAI) има през EMBED_PROVIDER=openai, но нито
една задача не се нуждае от него.

Пишем математиката с ОБИКНОВЕН Python, без библиотеки. Един вектор е просто
списък от числа:

    cos(a, b) = (a · b) / (||a|| * ||b||)
"""

import math
import os

EMBED_PROVIDER = os.environ.get("EMBED_PROVIDER", "ollama")

OLLAMA_MODEL = "bge-m3"  # 1024 измерения; многоезичен
OPENAI_MODEL = "text-embedding-3-small"  # 1536 измерения; изисква ключ


def dot(a, b) -> float:
    return sum(x * y for x, y in zip(a, b))


def norm(v) -> float:
    return math.sqrt(sum(x * x for x in v))


def cos_sim(a, b) -> float:
    return dot(a, b) / (norm(a) * norm(b))


def embed(texts: list[str], model: str | None = None) -> list[list[float]]:
    """Векторизира всички текстове наведнъж (batch), с една заявка. (дадено)"""
    if EMBED_PROVIDER == "openai":
        from openai import OpenAI

        resp = OpenAI().embeddings.create(model=model or OPENAI_MODEL, input=texts)
        return [d.embedding for d in resp.data]

    import ollama

    return ollama.embed(model=model or OLLAMA_MODEL, input=texts)["embeddings"]


def dim() -> int:
    """Размерност на текущия модел (свойство на модела, не на текста)."""
    return 1536 if EMBED_PROVIDER == "openai" else 1024
