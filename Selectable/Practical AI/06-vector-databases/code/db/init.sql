-- Изпълнява се автоматично при първия старт на контейнера.
-- Това е цялата „векторна база“: таблица с колона от тип vector и индекс върху нея.

CREATE EXTENSION IF NOT EXISTS vector;

-- Един запис = id + текст + метаданни + вектор (слайд „Структура на записа“).
-- Размерността 1024 е на bge-m3 и трябва да съвпада с DIM в db.py.
CREATE TABLE IF NOT EXISTS docs (
  id        text PRIMARY KEY,
  content   text,
  category  text,
  year      int,
  embedding vector(1024)
);

-- HNSW индекс за КОСИНУСОВО разстояние (операторът <=>).
CREATE INDEX IF NOT EXISTS docs_embedding_hnsw
  ON docs USING hnsw (embedding vector_cosine_ops);

-- Оператори: <=> косинусово разстояние, <-> евклидово (L2), <#> отриц. скаларно.
-- Това са РАЗСТОЯНИЯ: по-малко = по-близо. Косинусово сходство = 1 - (embedding <=> заявка).
