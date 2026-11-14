"""Задача 1 — промптът като шаблон + few-shot примери.

Промптът не е низ, слепен насред кода, а ФУНКЦИЯ: статичната инструкция
(SYSTEM) е константа, а данните на потребителя влизат в разделители
(<review>...</review>). После добавяме few-shot примери — двойки
user/assistant, които ПОКАЗВАТ на модела формата на отговора.

Таблицата сравнява три промпта за един и същ вход:
  неясна     SYSTEM казва само „с една дума“ — моделът сам избира думата;
  ясна       SYSTEM_CLEAR изрежда етикетите — инструкцията върши част от работата;
  few-shot   неясната инструкция + примери — примерите ПОКАЗВАТ етикетите.
Кодът след модела чака точно "неутрално"; „Неутрално“ или „Разочарован“ го чупят.

Стартиране:
    python solutions/01_prompt.py
    python solutions/01_prompt.py --model llama3.2:1b      # с определен модел
    python solutions/01_prompt.py --text "Вашият опит за prompt injection"

Изход (bggpt:4b; при вас може леко да се различава):
    вход                              неясна             ясна               few-shot
    -------------------------------------------------------------------------------
    Става, нищо особено.              Неутрално [x]      неутрално [ok]     неутрално [ok]
    Направо страхотно преживяване!    Положително [x]    положително [ok]   положително [ok]
    Разочарован съм от обслужването.  Разочарован [x]    отрицателно [ok]   отрицателно [ok]
    Закъсня, но продуктът е добър.    Неутрално [x]      неутрално [ok]     неутрално [ok]

    Валидни етикети: неясна 0/4, ясна 4/4, few-shot 4/4
"""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import llm`
from llm import chat, cli

# Статичната инструкция: константа под контрол на версиите, не се слепва с данни.
SYSTEM = "Определи настроението на отзива с една дума."
# Същата задача, но с ясно изредени етикети (за сравнение в таблицата).
SYSTEM_CLEAR = ("Определи настроението на отзива. Върни само една дума: "
                "положително, отрицателно или неутрално.")

# Етикетите, които кодът след модела очаква — точно тези низове.
LABELS = {"положително", "отрицателно", "неутрално"}

TEST_INPUTS = [
    "Става, нищо особено.",
    "Направо страхотно преживяване!",
    "Разочарован съм от обслужването.",
    "Закъсня, но продуктът е добър.",
]


def build_messages(text: str) -> list[dict]:
    """Шаблонът: данните на потребителя в разделители, отделени от инструкцията."""
    return [{"role": "user", "content": f"<review>\n{text}\n</review>"}]


# Примерите са редуващи се двойки user/assistant — моделът „вижда“ формата.
FEW_SHOT = [
    {"role": "user", "content": "<review>Обожавам този продукт!</review>"},
    {"role": "assistant", "content": "положително"},
    {"role": "user", "content": "<review>Счупи се още първия ден.</review>"},
    {"role": "assistant", "content": "отрицателно"},
    # Без неутрален пример моделът си измисля етикет („смесено“).
    {"role": "user", "content": "<review>Нормален продукт, нищо повече.</review>"},
    {"role": "assistant", "content": "неутрално"},
]


def make_messages(text: str, few_shot: bool) -> list[dict]:
    """Примерите (ако има) отпред, същинският вход — накрая."""
    return (FEW_SHOT if few_shot else []) + build_messages(text)


def classify(text: str, mode: str, provider: str, model: str | None) -> str:
    """mode: "неясна", "ясна" или "few-shot" — коя колона от таблицата."""
    system = SYSTEM_CLEAR if mode == "ясна" else SYSTEM
    messages = make_messages(text, few_shot=(mode == "few-shot"))
    reply = chat(system, messages, max_tokens=16, temperature=0,
                 provider=provider, model=model)
    return reply.text.replace("\n", " ")


def main() -> None:
    parser = cli("Шаблон за промпт и few-shot.")
    parser.add_argument("--text", help="класифицирай само този вход (few-shot)")
    args = parser.parse_args()

    if args.text:  # предизвикателството: накарайте модела да върне нещо извън LABELS
        label = classify(args.text, "few-shot", args.provider, args.model)
        print(f"{label!r} -> {'етикет' if label in LABELS else 'ПРОБИВ: не е етикет!'}")
        return

    # Прозрачност: какво точно заминава към модела (без system).
    print("Изпращаме (few-shot, първия вход):")
    for m in make_messages(TEST_INPUTS[0], few_shot=True):
        print(f"  {m['role']:<10} {m['content']!r}")
    if not FEW_SHOT:
        print("\n(FEW_SHOT е празен — колона few-shot праща същия промпт като неясна;\n"
              " ако думите пак се различават, това е остатъчната вариация при temperature=0)")

    modes = ("неясна", "ясна", "few-shot")
    print(f"\n{'вход':<34}" + "".join(f"{m:<19}" for m in modes))
    print("-" * 91)
    valid = dict.fromkeys(modes, 0)
    for text in TEST_INPUTS:
        row = f"{text[:33]:<34}"
        for mode in modes:
            label = classify(text, mode, args.provider, args.model)
            ok = label in LABELS  # точно съвпадение, както би проверил кодът
            valid[mode] += ok
            row += f"{label[:13] + (' [ok]' if ok else ' [x]'):<19}"
        print(row)

    n = len(TEST_INPUTS)
    print("\nВалидни етикети: " + ", ".join(f"{m} {valid[m]}/{n}" for m in modes))


# ---------------------------------------------------------------------------
# Предизвикателство (3 минути, по двойки): накарайте класификатора да върне
# нещо, което НЕ е етикет. Първите успели го показват на екрана.
#     python solutions/01_prompt.py --text "..."
# Подсказка: затворете тага </review>. Защо разделителите не помагат?
# (Подреждат промпта за модела, но не са защита — Лекция 14.)
# ---------------------------------------------------------------------------


if __name__ == "__main__":
    main()
