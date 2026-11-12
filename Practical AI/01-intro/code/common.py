"""Споделени помощници за Упражнение 1.

Тук живее всичко, което НЕ е предмет на задачите: четенето на ключа от
`.env`, създаването на клиент, ценовата таблица и редът с токени, латентност
и цена, който печатат всички скриптове.

Не е нужно да редактирате този файл — но го прегледайте набързо, за да
знаете какво ви е спестено.
"""

from __future__ import annotations

import os
import sys
from pathlib import Path

from dotenv import load_dotenv

CODE_DIR = Path(__file__).resolve().parent

# Два локални модела — нарочно, за да се види разликата:
#   llama3.2:1b  общ модел, 1B параметъра, ~1.3 GB — бърз, но зле на български
#   bggpt:4b     BgGPT на INSAIT (Gemma 3, 4B, Q4_K_M, ~2.5 GB) — адаптиран за
#                български; по-бавен, но свързан текст
# Така сравнението е тристранно: hosted / общ локален / езиково адаптиран локален.
# BgGPT се тегли от ОФИЦИАЛНОТО репо в Hugging Face и се преименува на кратко
# име. (В каталога на Ollama има и потребителски копия — ползваме официалното
# репо, за да знаем какви тегла и какъв шаблон получаваме.)
LOCAL_MODELS = ["llama3.2:1b", "bggpt:4b"]

# Откъде се тегли всеки локален модел (име в Ollama -> адрес за pull).
OLLAMA_SOURCES = {
    "llama3.2:1b": "llama3.2:1b",
    "bggpt:4b": "hf.co/INSAIT-Institute/BgGPT-Gemma-3-4B-IT-GGUF:Q4_K_M",
}

# Евтините/бързи варианти на всеки доставчик — подходящи за упражнението.
MODELS = {
    "anthropic": "claude-haiku-4-5",
    "ollama": "bggpt:4b",  # основният локален модел (Задача 4)
    # Само за домашното (Задача 4). Сверете актуалното име в документацията
    # на OpenAI — имената на моделите се сменят често.
    "openai": "gpt-6-luna",
}

# Приблизителни цени в USD за 1M токена.
# ВНИМАНИЕ: илюстративни са и остаряват — сверявайте с ценовите страници.
PRICES = {
    "claude-haiku-4-5": {"input": 1.00, "output": 5.00},
    # Локалните са 0 в токени, но носят реален хардуерен/енергиен разход.
    "llama3.2:1b": {"input": 0.0, "output": 0.0},
    "bggpt:4b": {"input": 0.0, "output": 0.0},
}

SYSTEM = "Ти си кратък и точен асистент. Отговаряй на български."

OLLAMA_URL = "http://localhost:11434"


def load_env() -> None:
    """Зарежда `code/.env` независимо от това откъде е стартиран скриптът."""
    load_dotenv(CODE_DIR / ".env")


def require_anthropic_key() -> None:
    """Спира с разбираемо съобщение, ако липсва ключ (вместо 401 по-късно).

    Два начина за идентификация пред API-то:
      * Console API ключ (sk-ant-api...) -> header x-api-key   <- обичайният случай
      * OAuth токен      (sk-ant-oat...) -> Authorization: Bearer
    SDK-то избира header-а само; тук само проверяваме, че поне единият е наличен.
    """
    load_env()
    if os.environ.get("ANTHROPIC_API_KEY") or os.environ.get("ANTHROPIC_AUTH_TOKEN"):
        return
    sys.exit(
        "Липсва ANTHROPIC_API_KEY (или ANTHROPIC_AUTH_TOKEN).\n"
        "  -> cp .env.example .env  и попълнете ключа си.\n"
        "  -> после проверете с: python check_setup.py"
    )


def anthropic_client():
    """Готов Anthropic клиент (с проверен ключ)."""
    import anthropic

    require_anthropic_key()
    return anthropic.Anthropic()


def auth_headers() -> dict[str, str]:
    """Header-ите за идентификация при ръчна HTTP заявка (Задача 1)."""
    load_env()
    api_key = os.environ.get("ANTHROPIC_API_KEY")
    if api_key:
        return {"x-api-key": api_key}
    token = os.environ.get("ANTHROPIC_AUTH_TOKEN")
    if token:
        return {"authorization": f"Bearer {token}"}
    require_anthropic_key()  # спира с инструкцията по-горе
    return {}


def ollama_available() -> bool:
    """True, ако локалният Ollama сървър отговаря."""
    import requests

    try:
        return requests.get(f"{OLLAMA_URL}/api/tags", timeout=1.5).status_code == 200
    except Exception:
        return False


def ollama_models() -> list[str]:
    """Имената на изтеглените локални модели (празен списък при проблем)."""
    import requests

    try:
        resp = requests.get(f"{OLLAMA_URL}/api/tags", timeout=1.5)
        return [m["name"] for m in resp.json().get("models", [])]
    except Exception:
        return []


def pull_commands(model: str) -> str:
    """Командите за изтегляне на даден локален модел (за съобщения за грешка)."""
    source = OLLAMA_SOURCES.get(model, model)
    if source == model:
        return f"ollama pull {model}"
    return f"ollama pull {source}\n     ollama cp   {source} {model}"


def ollama_has(model: str) -> bool:
    """True, ако моделът е изтеглен локално."""
    return any(name.startswith(model) for name in ollama_models())


def require_ollama(model: str | None = None) -> None:
    """Спира с инструкции, ако Ollama или конкретният модел не са готови."""
    model = model or MODELS["ollama"]
    if not ollama_available():
        sys.exit(
            "Ollama не отговаря на localhost:11434.\n"
            "  -> инсталация: https://ollama.com\n"
            "  -> после го пуснете (ollama serve) и изтеглете моделите:\n"
            + "\n".join(f"     {pull_commands(m)}" for m in LOCAL_MODELS)
        )
    if not ollama_has(model):
        sys.exit(f"Моделът {model} не е изтеглен.\n  -> {pull_commands(model)}")


def cost_usd(model: str, input_tokens: int, output_tokens: int) -> float:
    """Приблизителна цена на едно извикване в USD."""
    price = PRICES.get(model)
    if price is None:
        return 0.0
    return (input_tokens * price["input"] + output_tokens * price["output"]) / 1_000_000


def print_usage(provider: str, model: str, input_tokens: int, output_tokens: int,
                seconds: float) -> None:
    """Единният ред с токени, латентност и цена — печата се след всеки отговор.

    Благодарение на него не ни трябва отделна задача за сравнение: числата
    от hosted и от локалния модел се виждат при всяко пускане.
    """
    usd = cost_usd(model, input_tokens, output_tokens)
    print(
        f"[{provider}/{model}]  вход={input_tokens} изход={output_tokens}"
        f"  ·  {seconds:.2f}s  ·  ~${usd:.6f}"
    )
