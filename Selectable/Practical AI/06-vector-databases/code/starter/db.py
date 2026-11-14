"""Връзка към базата и embedding - даден код, не се променя.

Базата се вдига с `docker compose up -d`; таблицата и HNSW индексът се създават
автоматично от db/init.sql. Векторите идват от локалния Ollama (същият модел
като в Тема 5), така че всичко работи без API ключ.
"""

import os

import numpy as np
import ollama
import psycopg
from pgvector.psycopg import register_vector

DSN = os.environ.get("DATABASE_URL", "postgresql://postgres:postgres@localhost:5432/vectors")

MODEL = "bge-m3"   # многоезичен, от Тема 5: ollama pull bge-m3
DIM = 1024         # трябва да съвпада с vector(1024) в db/init.sql


def connect() -> psycopg.Connection:
    """Връзка към Postgres, която знае типа ``vector``."""
    conn = psycopg.connect(DSN)
    register_vector(conn)
    return conn


def embed(texts: list[str]) -> list[np.ndarray]:
    """По един вектор за всеки текст, с една заявка към Ollama."""
    vectors = ollama.embed(model=MODEL, input=texts)["embeddings"]
    return [np.array(v, dtype=np.float32) for v in vectors]
