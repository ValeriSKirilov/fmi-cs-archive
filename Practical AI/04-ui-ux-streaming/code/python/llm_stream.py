"""Споделен стрийминг слой за Упражнение 4. НЕ се редактира.

Всеки стриймър е АСИНХРОНЕН генератор, който дава текстови ДЕЛТИ (парчета от
отговора). Асинхронен е нарочно: когато браузърът прекъсне връзката, FastAPI
отменя задачата и `async for` се прекъсва в точката на изчакване, т.е. затваря
се и връзката към доставчика и генерацията спира (Задача 4).

Локалният модел е по подразбиране (без ключ); OpenAI и Anthropic са по избор и
работят със същия интерфейс. Ползва се от 00 (терминал), 01 (SSE сървър) и
02 (Generative UI).
"""

import os
from collections.abc import AsyncIterator, Iterator
from pathlib import Path

from dotenv import load_dotenv

# Зареждаме .env ТУК, преди MODELS да прочете OLLAMA_MODEL (иначе import-ът
# в 00/01/02 идва преди load_dotenv() и стойността от .env се губи).
load_dotenv(Path(__file__).with_name(".env"))


class SetupError(RuntimeError):
    """Липсва Ollama или моделът. Не е sys.exit(): иначе пада целият сървър."""


MODELS = {
    "ollama": os.environ.get("OLLAMA_MODEL", "bggpt:4b"),
    "openai": "gpt-6-luna",
    "anthropic": "claude-haiku-4-5",
}

# Кратки отговори: упражнението е за стрийминга, не за дълги текстове.
SYSTEM = "Отговаряй на български с едно или две кратки изречения."
MAX_TOKENS = 120


async def stream_ollama(prompt: str, system: str = SYSTEM) -> AsyncIterator[str]:
    import httpx
    import ollama

    try:
        stream = await ollama.AsyncClient().chat(
            model=MODELS["ollama"],
            messages=[
                {"role": "system", "content": system},
                {"role": "user", "content": prompt},
            ],
            options={"num_predict": MAX_TOKENS},
            stream=True,
        )
        async for chunk in stream:
            yield chunk["message"]["content"]
    except (ConnectionError, httpx.ConnectError):
        raise SetupError("Ollama не работи. Стартирайте я: ollama serve") from None
    except ollama.ResponseError as e:
        if e.status_code == 404:
            raise SetupError(f"Липсва моделът. Свалете го: ollama pull {MODELS['ollama']}") from None
        raise


async def stream_openai(prompt: str, system: str = SYSTEM) -> AsyncIterator[str]:
    from openai import AsyncOpenAI

    stream = await AsyncOpenAI().chat.completions.create(
        model=MODELS["openai"],
        messages=[{"role": "system", "content": system}, {"role": "user", "content": prompt}],
        max_completion_tokens=MAX_TOKENS,
        stream=True,
    )
    async for chunk in stream:
        if chunk.choices:
            yield chunk.choices[0].delta.content or ""


async def stream_anthropic(prompt: str, system: str = SYSTEM) -> AsyncIterator[str]:
    import anthropic

    async with anthropic.AsyncAnthropic().messages.stream(
        model=MODELS["anthropic"],
        max_tokens=MAX_TOKENS,
        system=system,
        messages=[{"role": "user", "content": prompt}],
    ) as stream:
        async for text in stream.text_stream:
            yield text


async def complete(provider: str, prompt: str) -> str:
    """Същото БЕЗ стрийминг: чакаме целия отговор (за сравнение в Задача 1)."""
    return "".join([d async for d in STREAMERS[provider](prompt)])


def stream_ollama_sync(prompt: str) -> Iterator[str]:
    """Синхронната версия от слайдовете (само за /chat_sync в Задача 4)."""
    import ollama

    for chunk in ollama.chat(
        model=MODELS["ollama"],
        messages=[{"role": "system", "content": SYSTEM}, {"role": "user", "content": prompt}],
        options={"num_predict": MAX_TOKENS},
        stream=True,
    ):
        yield chunk["message"]["content"]


STREAMERS = {
    "ollama": stream_ollama,
    "openai": stream_openai,
    "anthropic": stream_anthropic,
}
