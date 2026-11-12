"""Проверка на средата — пуснете това ПЪРВО (вкъщи, преди упражнението).

Проверява Python, пакетите, Ollama и модела, след което ГО ЗАРЕЖДА в паметта с едно кратко
извикване. Първото извикване на локален модел е бавно (зареждане); след него
отговорите тръгват бързо. Това е и първата ви среща с TTFT.

Стартиране:
    python check_setup.py
"""

import asyncio
import sys
import time

OK, FAIL = "✓", "✗"
results: list[bool] = []


def report(passed: bool, message: str, fix: str = "") -> bool:
    print(f"{OK if passed else FAIL} {message}")
    if not passed and fix:
        print(f"  -> {fix}")
    results.append(passed)
    return passed


def main() -> None:
    v = sys.version_info
    report(
        v >= (3, 10), f"Python {v.major}.{v.minor}.{v.micro}", "Нужен е Python 3.10+."
    )

    missing = []
    for mod, pkg in [
        ("fastapi", "fastapi"),
        ("uvicorn", "uvicorn"),
        ("ollama", "ollama"),
        ("dotenv", "python-dotenv"),
    ]:
        try:
            __import__(mod)
        except ImportError:
            missing.append(pkg)
    if not report(
        not missing,
        "Пакети" if not missing else f"Липсват: {', '.join(missing)}",
        "pip install -r requirements.txt  (в активирана виртуална среда)",
    ):
        sys.exit(1)

    from dotenv import load_dotenv

    load_dotenv()
    import ollama
    from llm_stream import MODELS, stream_ollama

    try:
        names = [m.model for m in ollama.list().models]
    except Exception:
        report(
            False,
            "Ollama отговаря",
            "Стартирайте я: ollama serve (или приложението Ollama)",
        )
        sys.exit(1)
    report(True, "Ollama отговаря")

    model = MODELS["ollama"]
    if not report(
        model in names or f"{model}:latest" in names,
        f"Моделът {model} е свален",
        f"ollama pull {model}",
    ):
        sys.exit(1)

    async def warm_up():
        start = time.perf_counter()
        first = None
        async for delta in stream_ollama("Кажи „готов“."):
            if delta and first is None:
                first = time.perf_counter() - start
        return first, time.perf_counter() - start

    first, total = asyncio.run(warm_up())
    report(
        first is not None,
        f"Моделът отговори (първи токен след {first:.1f}s, общо {total:.1f}s)"
        if first
        else "Моделът отговори",
        "Проверете ollama serve и модела.",
    )
    print(
        "\n"
        + (
            "Средата е готова за упражнението."
            if all(results)
            else "Има проблеми — вижте ✗ по-горе."
        )
    )
    sys.exit(0 if all(results) else 1)


if __name__ == "__main__":
    main()
