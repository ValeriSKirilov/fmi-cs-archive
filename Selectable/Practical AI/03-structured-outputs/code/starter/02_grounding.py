"""Задача 2: схемата е спазена, данните са измислени.

Задача 1 завърши с една гаранция: изходът ВИНАГИ е валиден JSON по схемата.
Тази задача показва докъде стига гаранцията.

Подаваме текст, в който липсва част от информацията. Схемата обаче изисква
четири низа. Моделът не може да върне по-малко полета, защото граматиката не
му позволява, затова ги ПОПЪЛВА:

    Вход:  „Ivan Petrov, CTO.“
    Изход: {"name": "Ivan Petrov", "email": "", "company": "Unknown", ...}
                                                           ^^^^^^^^^ в текста го няма

Типовете са верни. Но „Unknown“ не е компания, а „неизвестен“ не е имейл.
Затягането на схемата не помага: пробвахме `email` с минимална дължина и
моделът просто написа по-дълга измислица (Anthropic върна `<UNKNOWN>`).
Ограничението се спазва, стойността пак е боклук.

Изводът: схемата гарантира ФОРМАТА, не ИСТИННОСТТА. Истинността е отделна
проверка, пише се на ръка и е ваша работа. Каквото тя намери, се връща на
модела и се опитва пак (self-healing retry).

Стартиране:
    python starter/02_grounding.py
    python starter/02_grounding.py --model llama3.2:1b    # съчинява много повече

Изход СЛЕД TODO-тата (bggpt:4b):
    [2] „Съобщение от д-р Стоянова относно резултатите.“
       опит 1: email (не прилича на имейл адрес)
       опит 2: {'name': 'д-р Стоянова', 'email': '', 'company': '', 'role': ''}
       -> OK на опит 2

    [3] „Ivan Petrov, CTO.“
       опит 1: company (стойността я няма в текста)
       опит 2: {'name': 'Ivan Petrov', 'email': '', 'company': '', 'role': 'CTO'}
       -> OK на опит 2

Четвъртият случай е контролен: в него нищо не липсва и минава от първи опит.
Проверката не бива да вдига фалшива тревога.

Задължително пуснете и `--model llama3.2:1b`. Там моделът НЕ се поправя:

    [1] „Обади се на Петър утре сутринта.“
       опит 1: email (...), company (...), role (...)
       опит 2: същото.   опит 3: същото.
       -> изчерпани 3 опита; изчистени полета: ['email', 'company', 'role']

Не разбира поправката, а я имитира: в един от случаите слага буквално
„ГИ НЯМА“ в полето `name`. Поуката: retry е СТРАТЕГИЯ, не гаранция. Затова
има таван на опитите и явен fallback.
"""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import llm`
from llm import chat, cli

sys.path.insert(0, str(Path(__file__).resolve().parent))  # за `import schemas`
from schemas import Contact

SYSTEM = "Извличай структурирани данни от текста."
MAX_ATTEMPTS = 3

# Текстове с ЛИПСВАЩА информация. Последният е контролен: в него всичко е
# налице, така че проверката не бива да вдига фалшива тревога.
CASES = [
    "Обади се на Петър утре сутринта.",
    "Съобщение от д-р Стоянова относно резултатите.",
    "Ivan Petrov, CTO.",
    "Иван Петров, системен администратор във Фирма АД, ivan@firma.example.com",
]


def problems(contact: Contact, text: str) -> list[tuple[str, str]]:
    """Какво не е наред с извлечените данни. Връща двойки (поле, оплакване).

    Две неща, които схемата не може да провери:
      * имейл без „@“: структурата е вярна, съдържанието не е;
      * стойност, която изобщо я няма в изходния текст.
    Празните полета са наред, защото моделът честно е казал „не знам“.
    """
    low = text.lower()
    found = []
    for field, value in contact.model_dump().items():
        if not value:
            continue
        elif field == "email" and "@" not in value:
            found.append((field, "не прилича на имейл адрес"))
        elif value.lower() not in low:
            found.append((field, "стойността я няма в текста"))
            
    return found


def blank_out(contact: Contact, fields: list[str]) -> Contact:
    """Дадено. Явен fallback: изчистваме неподкрепените полета вместо да ги приемем."""
    data = contact.model_dump()
    for field in fields:
        data[field] = ""
    return Contact(**data)


def extract_grounded(text: str, args) -> tuple[Contact, int, list[str]]:
    """Извлича, после повтаря, докато `problems()` не върне празен списък."""
    messages: list[dict] = [{"role": "user", "content": text}]
    contact, issues = None, []

    for attempt in range(1, MAX_ATTEMPTS + 1):
        reply = chat(SYSTEM, messages, schema=Contact, temperature=0,
                     provider=args.provider, model=args.model)
        if reply.truncated:  # схемата не пази от отрязан отговор
            print("   [!] отговорът е отрязан (max_tokens), вдигнете тавана")
            break

        contact = Contact.model_validate_json(reply.text)
        issues = problems(contact, text)
        if not issues:
            print(f"   опит {attempt}: {contact.model_dump()}")
            return contact, attempt, []
        print(f"   опит {attempt}: " + ", ".join(f"{f} ({m})" for f, m in issues))

        messages += [
            {"role": "assistant", "content": reply.text},
            {"role": "user", "content": 
                "Върни празен низ за тези полета, които не минават проверка: "
                + "; ".join(f"{f}: ({v})" for f, v in issues)
                + ". Не измисляй и не превеждай."}
        ]

    fields = [f for f, _ in issues]
    return blank_out(contact, fields), MAX_ATTEMPTS, fields


def main() -> None:
    args = cli("Семантична проверка и self-healing retry.").parse_args()

    if not Contact.model_fields:
        sys.exit("`Contact` няма полета. Първо попълнете schemas.py (Задача 1).")


    for i, text in enumerate(CASES, start=1):
        print(f"\n[{i}] „{text}“")
        contact, attempts, failed = extract_grounded(text, args)
        if failed:
            print(f"   -> изчерпани {attempts} опита; изчистени полета: {failed}")
        else:
            print(f"   -> OK на опит {attempts}")

    print("\nСхемата гарантира ФОРМАТА, не ИСТИННОСТТА. Дали стойностите"
          "\nнаистина ги има в текста е отделна проверка и тя е ваша работа,"
          "\nкакто и решението какво да стане, когато моделът не се поправи."
          "\nТук изчистваме полето и го казваме на глас, вместо мълчаливо да"
          "\nприемем измислицата.")


if __name__ == "__main__":
    main()
