-- Изпълнява се автоматично при първи старт на pgvector контейнера.
-- Размерността (1024) е на bge-m3 и трябва да съвпада с модела в rag.py.
-- Ако смените модела, сменете и vector(1024), после: docker compose down -v

CREATE EXTENSION IF NOT EXISTS vector;

CREATE TABLE IF NOT EXISTS chunks (
  id          text PRIMARY KEY,   -- напр. support.md#2
  source      text,               -- файлът, от който идва частта (за цитиране)
  chunk_index int,                -- поредният номер на частта във файла
  content     text,               -- самият текст на частта
  embedding   vector(1024)
);

-- HNSW индекс за косинусово разстояние (Тема 6)
CREATE INDEX IF NOT EXISTS chunks_embedding_hnsw
  ON chunks USING hnsw (embedding vector_cosine_ops);
