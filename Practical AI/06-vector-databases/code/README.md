# Упражнение 6 - код

Векторна база с pgvector: една таблица и един индекс.

## Подготовка

```bash
python3 -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt

docker compose up -d       # PostgreSQL + pgvector; таблицата и индексът се създават сами
ollama pull bge-m3         # същият модел като в Тема 5 (ако още го нямате)
```

Ако сте вдигали базата с по-стара версия на упражнението, започнете на чисто:
`docker compose down -v && docker compose up -d`.

Цялата „векторна база“ е в `db/init.sql`: колона `embedding vector(1024)` и
`CREATE INDEX ... USING hnsw (embedding vector_cosine_ops)`.

## Задачите

| # | Файл | Какво правите |
|---|------|---------------|
| 1 | `01_ingest.py` | **upsert**: довършвате `INSERT ... ON CONFLICT` и зареждате 24-те документа |
| 2 | `02_search.py` | **query**: довършвате `ORDER BY` и `LIMIT` |
| 3 | `02_search.py` | **филтър по метаданни**: условие в `where` |
| 4 | `04_benchmark.py` | *за напредналите*: recall, HNSW срещу IVFFlat, филтри - виж по-долу |

```bash
python starter/01_ingest.py
python starter/02_search.py "превозно средство"
python starter/02_search.py "превозно средство" --category transport
```

`starter/` е с `TODO`; `solutions/` е за сверка. `db.py` (връзка + embedding) и
`corpus.py` (24 документа с метаданни) са дадени.

## Задача 4 - за напредналите

Свършихте рано? Тази задача е нарочно голяма: в 4г няма край.
Мерим компромисите от лекцията върху 50 000 вектора в отделна таблица `bench`
(`docs` не се пипа). `bench.py` е даден: генерира групирани синтетични вектори
(100 групи, 128 измерения), 20 заявки и HNSW индекс.

```bash
python starter/bench.py              # ~15 s
python starter/04_benchmark.py a     # 4а: точно срещу HNSW при различен ef_search
python starter/04_benchmark.py b     # 4б: IVFFlat срещу HNSW при различен probes
python starter/04_benchmark.py c     # 4в: филтър върху HNSW и iterative_scan
```

В `04_benchmark.py` пишете `exact`, `recall`, `measure` и трите части.
Примерен изход (числата варират с ±0.1 между пускания):

```text
точно (без индекс)     recall@10 = 1.00     20.5 ms
HNSW ef_search=10     recall@10 = 0.64      2.8 ms
HNSW ef_search=40     recall@10 = 0.89      2.8 ms
HNSW ef_search=200    recall@10 = 0.95      6.9 ms

IVFFlat (lists=50) построен за   0.8 s, 26 MB
  probes=1     recall@10 = 0.72      2.4 ms
  probes=50    recall@10 = 1.00     20.8 ms
HNSW построен за         9.8 s, 40 MB

WHERE category = 'c07' (1% от редовете), искаме 10 реда
  iterative_scan=off            редове =  0.0   recall@10 = 0.00      3.3 ms
  iterative_scan=relaxed_order  редове = 10.0   recall@10 = 0.74     45.2 ms
  iterative_scan=strict_order   редове = 10.0   recall@10 = 0.61     48.0 ms
```

### 4г - отворени експерименти (без решение)

Изберете какво ви интересува; всяко е отделен малък проект.

1. **Крива recall-латентност.** Начертайте HNSW и IVFFlat на една графика
   (matplotlib). Кой индекс е по-добър при еднаква латентност?
2. **Параметри при строене.** `WITH (m = 8|16|32, ef_construction = 32|64|128)`:
   време за строене, размер, recall при `ef_search = 40`.
3. **halfvec.** Индекс върху `(embedding::halfvec(128)) halfvec_cosine_ops` и заявка
   с `embedding::halfvec(128) <=> q::halfvec(128)`. Колко памет пестите и колко
   recall губите?
4. **Мащаб.** `python bench.py 10000`, `200000`, `500000`. Как расте времето на
   точното търсене и на HNSW? Сравнете със сметката от лекцията.
5. **Данните решават.** Сменете `NOISE` в `bench.py` (1.0, 3.0) или генерирайте
   напълно случайни вектори. Recall зависи от данните, не само от индекса.
6. **Pre-filtering.** Добавете B-tree индекс върху `category`. Ползва ли го
   планерът (`EXPLAIN`)? А частичен HNSW индекс `... WHERE category = 'c07'`?
7. **IVFFlat върху празна таблица.** Създайте индекса *преди* данните и измерете
   recall - грешката от лекцията, в числа.
8. **Истински данни.** Векторизирайте няколко хиляди изречения с `bge-m3` и
   повторете 4а. По-лесни или по-трудни са за ANN от синтетичните?

## Бонус към задачи 1-3 (без нов код)

1. **Разстояние или сходство.** Обърнете посоката на подредбата от задача 2 (`DESC`).
   Какво излиза най-отгоре? `<=>` е *разстояние*: по-малко = по-близо.
2. **Документът се промени.** Сменете текста на `d05` в `corpus.py`, например на
   „Новият електрически автобус вози пътници из града.“, и пуснете отново
   `01_ingest.py`. Редовете остават 24 (upsert обновява по `id`), а търсенето на
   „превозно средство“ вече намира автобуса на трето място.
