"""Задача 3: къде е грешката, в търсенето или в генерирането?

Без нов код. За всеки въпрос скриптът показва трите най-близки части и
отговора. Прочетете ги и решете за всеки въпрос:

  * Има ли сред намерените части такава, която съдържа отговора?
      - Не: грешката е в ТЪРСЕНЕТО. Моделът не може да отговори на нещо,
        което не е видял.
      - Да, но отговорът е грешен или непълен: грешката е в ГЕНЕРИРАНЕТО.
  * На кое място е частта с отговора и с какво сходство?

Въпроси:
  а) Сравнете заявката само с кода (E-1042) с въпроса, описан с думи.
     Кое намира правилната част по-уверено? Защо точните кодове са трудни за
     embeddings (Тема 5)?
  б) Пуснете 01_ingest.py --chunk-size 10 и после този скрипт отново. Какво
     се промени?

Тези слабости поправяме в Тема 8 (хибридно търсене, re-ranking).

Стартиране (след 01_ingest.py):
    python 03_diagnose.py
    python 03_diagnose.py --solutions        # с готовите решения, за сверка
"""

import sys
from pathlib import Path

# Вашият код (chunk.py, rag.py) е в starter/. С --solutions скриптът ползва
# готовите решения от solutions/, за сверка.
HERE = Path(__file__).parent
sys.path.insert(
    0, str(HERE / ("solutions" if "--solutions" in sys.argv else "starter"))
)

from dotenv import load_dotenv
from generate import generate
from rag import GROUNDED_SYSTEM, build_prompt, retrieve
from store import Store

QUESTIONS = [
    "Как се заявява отпуск?",  # описан с думи
    "E-1042",  # само кодът
    "Какво означава грешка E-1042?",  # кодът във въпрос
    "Какво да направя, ако векторите не пасват на индекса?",  # парафраза на E-3300
]


def main() -> None:
    load_dotenv()
    if not GROUNDED_SYSTEM or build_prompt("въпрос", []) is None:
        sys.exit("Първо решете задача 2 (rag.py).")

    store = Store()
    for q in QUESTIONS:
        hits = retrieve(store, q, k=3)
        print(f"\nВъпрос: {q}")
        for i, h in enumerate(hits, start=1):
            preview = h.text[:80].replace("\n", " ")
            print(f"  [{i}] {h.score:.3f}  {h.source}: {preview}...")
        print(f"  Отговор: {generate(GROUNDED_SYSTEM, build_prompt(q, hits))}")


if __name__ == "__main__":
    main()
