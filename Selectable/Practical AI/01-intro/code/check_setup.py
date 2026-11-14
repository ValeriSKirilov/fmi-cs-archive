"""Проверка на средата — пуснете това ПЪРВО, преди задачите.

Нищо не се пише тук. Скриптът проверява шест неща и при всеки провал казва
точно коя команда да изпълните.

Стартиране:
    python check_setup.py
"""

from __future__ import annotations

import os
import sys
import time
from pathlib import Path

CODE_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(CODE_DIR))

OK, FAIL, WARN = "✓", "✗", "!"

results: list[bool] = []


def report(passed: bool, message: str, fix: str = "") -> bool:
    print(f"{OK if passed else FAIL} {message}")
    if not passed and fix:
        print(f"  -> {fix}")
    results.append(passed)
    return passed


def check_python() -> bool:
    v = sys.version_info
    return report(
        v >= (3, 10),
        f"Python {v.major}.{v.minor}.{v.micro}",
        "Нужен е Python 3.10 или по-нов.",
    )


def check_packages() -> bool:
    required = ["anthropic", "requests", "dotenv", "ollama"]
    missing = []
    for name in required:
        try:
            __import__(name)
        except ImportError:
            missing.append("python-dotenv" if name == "dotenv" else name)
    return report(
        not missing,
        "Пакети: " + ", ".join(required) if not missing else f"Липсват: {', '.join(missing)}",
        "pip install -r requirements.txt  (във включена виртуална среда)",
    )


def check_env_file() -> bool:
    return report(
        (CODE_DIR / ".env").exists(),
        ".env намерен",
        "cp .env.example .env   после го редактирайте и сложете ключа си",
    )


def check_key_present() -> bool:
    from common import load_env

    load_env()
    key = os.environ.get("ANTHROPIC_API_KEY")
    token = os.environ.get("ANTHROPIC_AUTH_TOKEN")
    value = key or token
    if not value or value.startswith(("sk-ant-...", "sk-...")):
        return report(False, "ANTHROPIC_API_KEY не е попълнен",
                      "Вземете ключ от https://console.anthropic.com и го сложете в .env")
    masked = f"{value[:11]}…{value[-4:]}"
    label = "ANTHROPIC_API_KEY" if key else "ANTHROPIC_AUTH_TOKEN"
    return report(True, f"{label}: {masked}")


def check_api_call() -> bool:
    """Най-важната проверка: ключът не просто е сложен, а наистина работи.

    Струва 1 токен (под $0.000002), но спестява дебъгване насред задачата.
    """
    if not results[-1]:
        return report(False, "Заявка до Anthropic — пропусната (няма ключ)")
    try:
        import anthropic

        from common import MODELS

        start = time.perf_counter()
        anthropic.Anthropic().messages.create(
            model=MODELS["anthropic"],
            max_tokens=1,
            messages=[{"role": "user", "content": "ok"}],
        )
        return report(True, f"Заявка до Anthropic: OK ({time.perf_counter() - start:.1f}s)")
    except Exception as exc:
        return report(False, f"Заявка до Anthropic се провали: {type(exc).__name__}: {exc}",
                      "Проверете ключа, кредита в акаунта и интернет връзката.")


def check_ollama() -> bool:
    """Проверява сървъра и ДВАТА локални модела (общия и българския)."""
    from common import LOCAL_MODELS, ollama_available, ollama_has, pull_commands

    if not ollama_available():
        return report(
            False, "Ollama не отговаря на localhost:11434",
            "Инсталирайте от https://ollama.com и го пуснете (ollama serve), после:\n  -> "
            + "\n  -> ".join(pull_commands(m).replace("\n     ", "\n  -> ") for m in LOCAL_MODELS),
        )
    all_ok = True
    for model in LOCAL_MODELS:
        ok = report(
            ollama_has(model),
            f"Ollama: {model} е готов" if ollama_has(model) else f"Ollama: {model} не е изтеглен",
            pull_commands(model).replace("\n     ", "\n  -> "),
        )
        all_ok = all_ok and ok
    return all_ok


def main() -> None:
    print("Проверка на средата за Упражнение 1\n")
    check_python()
    if not check_packages():
        print("\nБез пакетите нататък няма смисъл. Инсталирайте ги и пуснете пак.")
        sys.exit(1)
    check_env_file()
    check_key_present()
    check_api_call()
    check_ollama()

    passed = sum(results)
    print(f"\n{passed}/{len(results)} ✓")
    if passed == len(results):
        print("Средата е готова — започвайте със starter/01_http_vs_sdk.py")
    elif all(results[:5]):
        print(f"{WARN} Задачи 1, 3 и 4 ще работят. Задача 2 изисква Ollama и двата локални модела.")
    else:
        print(f"{WARN} Поправете маркираното по-горе и пуснете пак: python check_setup.py")
        sys.exit(1)


if __name__ == "__main__":
    main()
