"""Задача 3 — управление на контекста на ръка: прозорец и резюме.

Моделът няма памет — „паметта“ е списъкът `history`, който подаваме всеки път
(Упражнение 1). Той расте с всяка реплика, а с него и токените. Когато
историята надхвърли MAX_MESSAGES, я свиваме до последните KEEP съобщения:

  --mode none     историята не се свива (за сравнение: всичко се помни, но
                  „вход=“ расте с всяка реплика);
  --mode window   старите съобщения просто изчезват (sliding window);
  --mode summary  старите съобщения първо се свиват в кратко РЕЗЮМЕ, което
                  отива в system промпта (хибрид: резюме + последните KEEP).

--demo изпраща готов разговор от 6 реплики, за да не пишете на ръка.
В първата реплика казваме името на котката, в петата го питаме. Сравнете
отговора в двата режима. Шестата реплика пита за ДЕТАЙЛ (дословния втори
въпрос) — помни ли го резюмето? За сравнение пуснете и --mode none.

Стартиране:
    python solutions/03_memory.py --demo --mode window
    python solutions/03_memory.py --demo --mode summary
    python solutions/03_memory.py --demo --mode none     # без свиване, за сравнение
    python solutions/03_memory.py                      # интерактивен чат

Очакван изход (съкратен; вашите формулировки ще се различават):
    --mode window
      (историята е свита: 8 -> 4 съобщения)
      вие> Как се казва котката ми?
      бот> Не знам котката ти, но ако искаш да ѝ дадеш име, можеш да
           използваш функцията `input()` ...
    --mode summary
      (историята е свита: 8 -> 4 съобщения)
      (резюме: Потребителят има котка на име Мърка и иска да учи ...)
      вие> Как се казва котката ми?
      бот> Твоята котка се казва Мърка.
      вие> Какъв беше вторият въпрос, който ти зададох? Цитирай го дословно.
      бот> "Дай ми идея за малък проект с него."      <- грешно: това е четвъртият
    --mode none
      бот> "Кой език за програмиране да уча първо?"   <- вярно: пълната история
"""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import llm`
from llm import chat, cli, print_usage

SYSTEM = "Ти си полезен асистент. Отговаряй с едно кратко изречение, на български."

KEEP = 4          # последните съобщения, които пазим дословно (2 двойки въпрос/отговор)
MAX_MESSAGES = 6  # над толкова съобщения свиваме историята

DEMO = [
    "Здравей! Имам котка на име Мърка и искам да се науча да програмирам.",
    "Кой език за програмиране да уча първо?",
    "Защо точно него?",
    "Дай ми идея за малък проект с него.",
    "Как се казва котката ми?",
    "Какъв беше вторият въпрос, който ти зададох? Цитирай го дословно.",
]


def summarize(previous: str, messages: list[dict], provider: str, model: str | None) -> str:
    """Обновява резюмето с нови реплики (едно допълнително извикване на модела)."""
    transcript = "\n".join(f"{m['role']}: {m['content']}" for m in messages)
    reply = chat(
        "Обнови резюмето на разговора в 2–3 изречения. Запази имената и фактите "
        "за потребителя.",
        [{"role": "user", "content":
          f"<резюме>\n{previous}\n</резюме>\n<нови_реплики>\n{transcript}\n</нови_реплики>"}],
        max_tokens=200, temperature=0, provider=provider, model=model,
    )
    print("   (цената на резюмето — едно допълнително извикване:)")
    print_usage(reply)
    return reply.text


def compress(history: list[dict], summary: str, mode: str,
             provider: str, model: str | None) -> tuple[list[dict], str]:
    """Вика се след всеки отговор. Връща (новата история, новото резюме)."""
    if len(history) <= MAX_MESSAGES:
        return history, summary  # още няма нужда от свиване

    # Тук len(history) е четно (току-що добавихме отговор), а KEEP е четно,
    # затова history[-KEEP:] винаги започва със съобщение на user.
    if mode == "summary":
        summary = summarize(summary, history[:-KEEP], provider, model)
    return history[-KEEP:], summary


def build_system(summary: str) -> str:
    """System промптът: инструкцията + резюмето на по-ранния разговор (ако има)."""
    if summary:
        return SYSTEM + f"\n\nРезюме на разговора досега:\n{summary}"
    return SYSTEM


def main() -> None:
    parser = cli("Чат с управление на контекста.")
    parser.add_argument("--mode", choices=("none", "window", "summary"), default="summary")
    parser.add_argument("--demo", action="store_true", help="изпрати готовия разговор")
    args = parser.parse_args()

    history: list[dict] = []
    summary = ""
    inputs = iter(DEMO) if args.demo else None

    print(f"Чат, режим {args.mode}. 'изход' или Ctrl-D спира.\n")
    while True:
        if inputs is not None:
            user_in = next(inputs, None)
            if user_in is None:
                break
            print(f"вие> {user_in}")
        else:
            try:
                user_in = input("вие> ").strip()
            except EOFError:
                break
            if not user_in or user_in.lower() in {"изход", "exit", "quit"}:
                break

        history.append({"role": "user", "content": user_in})
        # temperature=0: демото да дава един и същ резултат при всяко пускане
        reply = chat(build_system(summary), history, max_tokens=150, temperature=0,
                     provider=args.provider, model=args.model)
        history.append({"role": "assistant", "content": reply.text})
        print(f"бот> {reply.text}")
        print_usage(reply)

        before = len(history)
        if args.mode != "none":  # none: без свиване, за сравнение
            history, summary = compress(history, summary, args.mode,
                                        args.provider, args.model)
        if len(history) < before:
            print(f"   (историята е свита: {before} -> {len(history)} съобщения)")
            if summary:
                print(f"   (резюме: {summary})")
        print()


if __name__ == "__main__":
    main()
