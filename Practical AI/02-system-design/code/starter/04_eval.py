"""Бонус — мини eval на класификатора от Задача 1.

В Задача 1 гледахме дали етикетът е ВАЛИДЕН (една от трите думи). Но валиден
не значи ВЕРЕН: модел, който винаги отговаря „отрицателно“, е 100% валиден и
наполовина грешен. Eval-ът пуска 8 отзива с известен етикет и брои и двете,
плюс латентност и токени. Без eval всяка промяна на промпт или модел крие ТИХА
регресия. При грешка скриптът завършва с код 1 — удобно за CI.

Единственото, което се пише, е `normalize()`: „Неутрално.“ и „неутрално“ са
един и същ отговор, но `==` не мисли така. Колко от проблема е в промпта и
колко — в липсващата обработка на изхода? Пуснете --zero-shot преди и след.

Скриптът се пуска и ПРЕДИ да сте написали нещо — normalize() просто още не
прави нищо. Пуснете --zero-shot сега и запомнете колко са валидните.

Стартиране:
    python starter/04_eval.py                         # промптът с примери
    python starter/04_eval.py --zero-shot             # без примери
    python starter/04_eval.py --model llama3.2:1b     # друг модел

Изход след TODO (bggpt:4b):
    --zero-shot, преди TODO   Валидни: 1/8   Верни: 1/8   („Положително“ ≠ „положително“)
    --zero-shot, след TODO    Валидни: 6/8   Верни: 6/8   (остава „огорчен“ ×2)
    с примери                 Валидни: 8/8   Верни: 8/8   (~3× повече входни токени)
    --model llama3.2:1b       Валидни: 8/8   Верни: 3/8   („отрицателно“ на всичко)
"""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import llm`
from llm import chat, cli

# Същият промпт като в Задача 1.
SYSTEM = "Определи настроението на отзива с една дума."
LABELS = {"положително", "отрицателно", "неутрално"}
FEW_SHOT = [
    {"role": "user", "content": "<review>Обожавам този продукт!</review>"},
    {"role": "assistant", "content": "положително"},
    {"role": "user", "content": "<review>Счупи се още първия ден.</review>"},
    {"role": "assistant", "content": "отрицателно"},
    {"role": "user", "content": "<review>Нормален продукт, нищо повече.</review>"},
    {"role": "assistant", "content": "неутрално"},
]

# (отзив, верният етикет) — нови отзиви, които моделът не е виждал като примери.
# TODO за напреднали (без готово решение): добавете 2 свои случая, поне един,
#   който моделът проваля. Защо го проваля — промптът, моделът или normalize()?
CASES = [
    ("Супер бърза доставка, препоръчвам!", "положително"),
    ("Най-добрата покупка за годината.", "положително"),
    ("Много съм доволна от качеството.", "положително"),
    ("Не работи, връщам го.", "отрицателно"),
    ("Ужасно обслужване, никога повече.", "отрицателно"),
    ("Платих много, а получих боклук.", "отрицателно"),
    ("Пристигна във вторник.", "неутрално"),
    ("Предлага се в три цвята.", "неутрално"),
]


def normalize(text: str) -> str:
    """Изчиства дребните разлики във формата: интервали, главни букви, точка."""
    # TODO: върнете text без интервалите в краищата, с малки букви и без
    #       точка/удивителна накрая. Един ред с методите на str.
    return text


def main() -> None:
    parser = cli("Мини eval на класификатора.")
    parser.add_argument("--zero-shot", action="store_true", help="без few-shot примери")
    args = parser.parse_args()
    examples = [] if args.zero_shot else FEW_SHOT

    header = f"{'отзив':<36} {'очаквано':<12} {'отговор':<16} {'резултат':<10} {'време':>6}  токени"
    print(header)
    print("-" * len(header))

    valid = correct = 0
    for review, expected in CASES:
        # temperature=0: eval-ът иска повторяемост, иначе мерим шум, а не регресии.
        reply = chat(SYSTEM, examples + [{"role": "user", "content": f"<review>\n{review}\n</review>"}],
                     max_tokens=16, temperature=0, provider=args.provider, model=args.model)
        label = normalize(reply.text)
        is_valid, is_correct = label in LABELS, label == expected
        valid += is_valid
        correct += is_correct
        result = "OK" if is_correct else ("ГРЕШЕН" if is_valid else "НЕВАЛИДЕН")
        print(f"{review:<36} {expected:<12} {label[:15]:<16} {result:<10} "
              f"{reply.seconds:>5.1f}s  {reply.input_tokens}/{reply.output_tokens}")

    print("-" * len(header))
    print(f"Валидни: {valid}/{len(CASES)}   Верни: {correct}/{len(CASES)}")
    if correct < len(CASES):
        sys.exit(1)  # ненулев код -> CI спира


if __name__ == "__main__":
    main()
