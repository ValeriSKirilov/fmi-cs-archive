"""GENERATE стъпката: извикване на модела (от Теми 1 и 2). Даден код, не е задача.

По подразбиране е ЛОКАЛЕН: BgGPT през Ollama, офлайн и без ключ. Облачен вариант
се избира с GEN_PROVIDER=openai или anthropic в .env. Три функции:
  generate()       цял отговор като текст
  stream()         отговорът парче по парче (стрийминг, Тема 4)
  generate_json()  структуриран изход по Pydantic схема (Тема 3)
"""

import os
from collections.abc import Iterator

from pydantic import BaseModel

GEN_PROVIDER = os.environ.get("GEN_PROVIDER", "ollama")

MODELS = {
    "ollama": "bggpt:4b",  # BgGPT на INSAIT, вж. README за инсталация
    "openai": "gpt-6-luna",
    "anthropic": "claude-haiku-4-5",
}


def _messages(system: str, user: str) -> list[dict]:
    return [{"role": "system", "content": system}, {"role": "user", "content": user}]


def _openai_kwargs(temperature: float) -> dict:
    # GPT-6 са reasoning модели: temperature се приема само без разсъждение (Тема 2).
    return {"reasoning_effort": "none", "temperature": temperature}


def _anthropic_kwargs(temperature: float) -> dict:
    # Haiku 4.5 приема temperature, но през extra_body (Тема 2).
    return {"extra_body": {"temperature": temperature}}


def generate(
    system: str, user: str, temperature: float = 0.1, max_tokens: int = 800
) -> str:
    if GEN_PROVIDER == "openai":
        from openai import OpenAI

        resp = OpenAI().chat.completions.create(
            model=MODELS["openai"],
            messages=_messages(system, user),
            max_completion_tokens=max_tokens,
            **_openai_kwargs(temperature),
        )
        return resp.choices[0].message.content

    if GEN_PROVIDER == "anthropic":
        import anthropic

        msg = anthropic.Anthropic().messages.create(
            model=MODELS["anthropic"],
            max_tokens=max_tokens,
            system=system,
            messages=[{"role": "user", "content": user}],
            **_anthropic_kwargs(temperature),
        )
        return next((b.text for b in msg.content if b.type == "text"), "")

    import ollama

    resp = ollama.chat(
        model=MODELS["ollama"],
        messages=_messages(system, user),
        options={"temperature": temperature, "num_predict": max_tokens},
    )
    return resp["message"]["content"]


def stream(system: str, user: str, temperature: float = 0.1) -> Iterator[str]:
    if GEN_PROVIDER == "openai":
        from openai import OpenAI

        for chunk in OpenAI().chat.completions.create(
            model=MODELS["openai"],
            messages=_messages(system, user),
            stream=True,
            **_openai_kwargs(temperature),
        ):
            if chunk.choices:
                yield chunk.choices[0].delta.content or ""
    elif GEN_PROVIDER == "anthropic":
        import anthropic

        with anthropic.Anthropic().messages.stream(
            model=MODELS["anthropic"],
            max_tokens=800,
            system=system,
            messages=[{"role": "user", "content": user}],
            **_anthropic_kwargs(temperature),
        ) as s:
            yield from s.text_stream
    else:
        import ollama

        for chunk in ollama.chat(
            model=MODELS["ollama"],
            messages=_messages(system, user),
            stream=True,
            options={"temperature": temperature},
        ):
            yield chunk["message"]["content"]


def generate_json(system: str, user: str, schema: type[BaseModel]) -> BaseModel:
    """Структуриран изход (Тема 3): моделът е ограничен да върне JSON по схемата."""
    if GEN_PROVIDER == "openai":
        from openai import OpenAI

        resp = OpenAI().chat.completions.parse(
            model=MODELS["openai"],
            messages=_messages(system, user),
            response_format=schema,
            **_openai_kwargs(0),
        )
        return resp.choices[0].message.parsed

    if GEN_PROVIDER == "anthropic":
        # Без ограничение по схема: молим за JSON и го валидираме.
        return schema.model_validate_json(generate(system, user, temperature=0))

    import ollama

    resp = ollama.chat(
        model=MODELS["ollama"],
        messages=_messages(system, user),
        format=schema.model_json_schema(),
        options={"temperature": 0},
    )
    return schema.model_validate_json(resp["message"]["content"])
