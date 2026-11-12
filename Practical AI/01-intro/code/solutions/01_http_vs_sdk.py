"""Задача 1 — едно и също извикване на две нива на абстракция.

`ask_raw()` праща заявката „на голо" с `requests`: URL, header-и, JSON тяло и
ръчно вадене на текста. `ask_sdk()` прави същото през официалния SDK.
Двете функции връщат еднакъв резултат — разликата е само в това колко
детайли виждате.

Стартиране:
    python solutions/01_http_vs_sdk.py "Коя е столицата на България?"

Очакван изход (вашите числа ще се различават):
    --- суров HTTP ---
    Столицата на България е **София**.
    [anthropic/claude-haiku-4-5]  вход=40 изход=13  ·  0.72s  ·  ~$0.000105

    --- SDK ---
    Столицата на България е **София**.
    [anthropic/claude-haiku-4-5]  вход=40 изход=13  ·  0.68s  ·  ~$0.000105
"""

from __future__ import annotations

import sys
import time
from pathlib import Path

import requests

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import common`
from common import MODELS, SYSTEM, anthropic_client, auth_headers, print_usage

MODEL = MODELS["anthropic"]
URL = "https://api.anthropic.com/v1/messages"
MAX_TOKENS = 512

Result = tuple[str, int, int, float]  # текст, токени вход, токени изход, секунди


def ask_raw(question: str) -> Result:
    """Суров HTTP POST — всичко, което SDK-то обикновено крие от вас."""
    start = time.perf_counter()
    response = requests.post(
        URL,
        headers={
            **auth_headers(),  # x-api-key: sk-ant-api...  (вж. common.py)
            "anthropic-version": "2023-06-01",  # версия на API договора, не дата на днес
            "content-type": "application/json",
        },
        json={
            "model": MODEL,
            "max_tokens": MAX_TOKENS,  # таван на ОТГОВОРА; при Anthropic е задължителен
            "system": SYSTEM,  # отделно поле, НЕ съобщение в messages
            "messages": [{"role": "user", "content": question}],
        },
        timeout=60,
    )
    response.raise_for_status()  # 4xx/5xx -> изключение вместо тих провал
    data = response.json()
    return (
        data["content"][0]["text"],  # текстът се вади на ръка от вложения JSON
        data["usage"]["input_tokens"],
        data["usage"]["output_tokens"],
        time.perf_counter() - start,
    )


# ---------------------------------------------------------------------------
# Преди да продължите, намерете в ask_raw() отговорите на:
#   1) На кой ред заминава ключът и в кой header?
#   2) Кой параметър ограничава дължината на ОТГОВОРА (а не на разговора)?
#   3) Защо `system` не е в списъка `messages`? Сравнете с OpenAI от лекцията.
# ---------------------------------------------------------------------------


def ask_sdk(question: str) -> Result:
    """Същата заявка през официалния `anthropic` SDK."""
    client = anthropic_client()
    start = time.perf_counter()
    message = client.messages.create(
        model=MODEL,
        max_tokens=MAX_TOKENS,
        system=SYSTEM,
        messages=[{"role": "user", "content": question}],
    )
    return (
        message.content[0].text,
        message.usage.input_tokens,
        message.usage.output_tokens,
        time.perf_counter() - start,
    )


def main() -> None:
    question = " ".join(sys.argv[1:]) or "Коя е столицата на България?"

    for label, fn in (("суров HTTP", ask_raw), ("SDK", ask_sdk)):
        text, input_tokens, output_tokens, seconds = fn(question)
        print(f"\n--- {label} ---")
        print(text)
        print_usage("anthropic", MODEL, input_tokens, output_tokens, seconds)

    print(
        "\nДве имплементации, едно и също API. SDK-то пое: header-а с ключа,"
        "\nanthropic-version, сериализацията към JSON, парсването на отговора"
        "\nи повторните опити при временни грешки."
        "\n\nОпитайте и с по-отворен въпрос, напр.:"
        "\n  python solutions/01_http_vs_sdk.py 'Дай три факта за езика Python.'"
        "\nПуснете го два-три пъти: отговорите ще се различават при един и същ"
        "\nвход. Моделът е недетерминиран — проектирайте толерантно към вариация."
    )


if __name__ == "__main__":
    main()
