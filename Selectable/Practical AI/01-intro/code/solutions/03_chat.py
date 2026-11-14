"""Задача 3 — роли и контекст: разговор от няколко реплики.

Досега всяко извикване беше едно самотно съобщение с роля `user`. Истинският
разговор е СПИСЪК от съобщения, в който се редуват `user` и `assistant`.

Ключовото прозрение: API-то е **stateless** — то не помни нищо между
заявките. Историята е ваша отговорност и се препраща ЦЯЛАТА при всяко
извикване. Затова токените на входа растат с всяка реплика (гледайте реда с
usage), а с тях расте и цената.

Стартиране:
    python solutions/03_chat.py

Очакван изход:
    > Казвам се Иван.
    Приятно ми е, Иван!
    [anthropic/claude-haiku-4-5]  вход=28 изход=8  ·  0.70s  ·  ~$0.000068
    > Как се казвам?
    Казваш се Иван.
    [anthropic/claude-haiku-4-5]  вход=48 изход=6  ·  0.65s  ·  ~$0.000078
"""

from __future__ import annotations

import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import common`
from common import MODELS, SYSTEM, anthropic_client, print_usage

MODEL = MODELS["anthropic"]
MAX_TOKENS = 512

Result = tuple[str, int, int, float]  # текст, токени вход, токени изход, секунди


def send(client, messages: list[dict]) -> Result:
    """Праща ЦЕЛИЯ списък съобщения и връща отговора на модела.

    Забележете: `system` е отделен параметър и се подава всеки път; ролите
    `user`/`assistant` живеят в `messages`.
    """
    start = time.perf_counter()
    message = client.messages.create(
        model=MODEL,
        max_tokens=MAX_TOKENS,
        system=SYSTEM,
        messages=messages,
    )
    return (
        message.content[0].text,
        message.usage.input_tokens,
        message.usage.output_tokens,
        time.perf_counter() - start,
    )


def main() -> None:
    client = anthropic_client()
    messages: list[dict] = []

    print("Мини-разговор. `/нов` изчиства контекста, `изход` спира.\n")

    while True:
        try:
            user_text = input("> ").strip()
        except (EOFError, KeyboardInterrupt):
            print()
            break

        if not user_text:
            continue
        if user_text in ("изход", "exit", "quit"):
            break
        if user_text == "/нов":
            messages = []
            print("(контекстът е изчистен — моделът вече не помни нищо)\n")
            continue

        messages.append({"role": "user", "content": user_text})
        reply, input_tokens, output_tokens, seconds = send(client, messages)
        print(reply)
        messages.append({"role": "assistant", "content": reply})
        print_usage("anthropic", MODEL, input_tokens, output_tokens, seconds)

    print(f"\nРазговорът завърши с {len(messages)} съобщения в историята.")


# ---------------------------------------------------------------------------
# Въпроси към наблюденията (обсъждаме ги накрая):
#   1) Защо токените на ВХОДА растат с всяка реплика? Колко ще са след 50?
#   2) Какво ще се случи, когато историята надмине контекстния прозорец на
#      модела? (Подсказка: max_tokens ограничава само ИЗХОДА.)
#   3) Какво точно прави `/нов` — и защо след него моделът „забравя"?
# Стратегиите за справяне (отрязване, резюмиране, подбор) са в Тема 2.
# ---------------------------------------------------------------------------


if __name__ == "__main__":
    main()
