"""STORE стъпката: векторното хранилище е pgvector (както в Тема 6), през Docker.

Даден код, не е задача. Таблицата chunks се създава от db/init.sql при първия
старт на контейнера: docker compose up -d
"""

import os
from dataclasses import dataclass

import numpy as np


@dataclass
class Hit:
    id: str
    source: str        # файлът, от който идва частта
    chunk_index: int   # поредният номер на частта във файла
    text: str
    score: float       # косинусово сходство: по-голямо значи по-близо


class Store:
    def __init__(self) -> None:
        import psycopg
        from pgvector.psycopg import register_vector

        url = os.environ.get("DATABASE_URL",
                             "postgresql://postgres:postgres@localhost:5432/vectors")
        self.conn = psycopg.connect(url)
        register_vector(self.conn)

    def clear(self) -> None:
        """Изтрива всички части, за да индексираме наново (напр. с друг размер)."""
        with self.conn.cursor() as cur:
            cur.execute("DELETE FROM chunks")
        self.conn.commit()

    def upsert(self, chunks, vectors) -> None:
        with self.conn.cursor() as cur:
            for c, v in zip(chunks, vectors):
                cur.execute(
                    """INSERT INTO chunks (id, source, chunk_index, content, embedding)
                       VALUES (%s, %s, %s, %s, %s)
                       ON CONFLICT (id) DO UPDATE SET
                         content = EXCLUDED.content, embedding = EXCLUDED.embedding""",
                    (c.id, c.source, c.index, c.text, np.asarray(v, dtype=np.float32)),
                )
        self.conn.commit()

    def query(self, vector, top_k: int = 4) -> list[Hit]:
        vec = np.asarray(vector, dtype=np.float32)
        with self.conn.cursor() as cur:
            cur.execute(
                """SELECT id, source, chunk_index, content, 1 - (embedding <=> %s) AS sim
                   FROM chunks ORDER BY embedding <=> %s LIMIT %s""",
                (vec, vec, top_k),
            )
            return [Hit(r[0], r[1], r[2], r[3], float(r[4])) for r in cur.fetchall()]
