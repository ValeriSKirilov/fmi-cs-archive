"""Задача 2 — същата форма съобщения-с-роли, но срещу локални модели.

Нищо в структурата на заявката не се променя: пак списък от съобщения с
роли. Сменя се само къде се изпълнява моделът — и как се казват полетата с
токените.

Скриптът пуска един и същ въпрос през ДВА локални модела:
    llama3.2:1b   общ модел, 1B параметъра — бърз, но зле на български
    bggpt:4b      BgGPT (INSAIT), Gemma 3, 4B — адаптиран за български
Изисква и двата изтеглени (вж. README; проверете с `python check_setup.py`).

Стартиране:
    python solutions/02_local.py "Дай три факта за езика Python."

Очакван изход (вашите числа и формулировки ще се различават):
    --- llama3.2:1b ---
    Исчестява кралство България, а след ... Книгополье стави сед shops capitale
    Bucarestul.
    [ollama/llama3.2:1b]  вход=46 изход=37  ·  3.52s  ·  ~$0.000000

    --- bggpt:4b ---
    Столицата на България е **София**. Тя е най-големият град в страната и
    важен културен, политически и икономически център...
    [ollama/bggpt:4b]  вход=23 изход=301  ·  7.64s  ·  ~$0.000000

Да, първият наистина връща такава безсмислица на български. 1B параметъра не
стигат за езика; моделът меси български, руски, френски и английски.
"""

from __future__ import annotations

import sys
import time
from pathlib import Path

import ollama

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import common`
from common import (
    LOCAL_MODELS,
    ollama_available,
    ollama_has,
    print_usage,
    pull_commands,
    require_ollama,
)

Result = tuple[str, int, int, float]  # текст, токени вход, токени изход, секунди


def ask_local(question: str, model: str) -> Result:
    """Извиква локален модел през Ollama."""
    start = time.perf_counter()
    response = ollama.chat(
        model=model,
        messages=[{"role": "user", "content": question}],
    )
    return (
        response["message"]["content"],
        response["prompt_eval_count"] or 0,
        response["eval_count"] or 0,
        time.perf_counter() - start,
    )


# ---------------------------------------------------------------------------
# Какво да сравните:
#   * С Задача 1: messages=[{"role": ..., "content": ...}] е ИДЕНТИЧНО.
#     Формата на chat API-то е де факто стандарт, независимо къде се изпълнява
#     моделът. Но токените се казват prompt_eval_count / eval_count вместо
#     input_tokens / output_tokens — заради такива дребни разлики съществува
#     Задача 4 (адаптерът).
#   * Между двата локални модела: llama3.2:1b е по-бърз, но на български пише
#     неграмотно или минава на английски; bggpt:4b е по-бавен, но свързан.
#     Изводът НЕ е „локалното е по-лошо", а „размерът и езиковата адаптация
#     решават" — общ модел от 1B просто няма капацитет за български.
#   * С hosted: няма ключ, няма мрежа, 0 на токен — но има реален хардуерен и
#     енергиен разход, и латентността е съвсем различна.
# ---------------------------------------------------------------------------


def main() -> None:
    if not ollama_available():
        require_ollama()  # спира с инструкции вместо stack trace

    question = " ".join(sys.argv[1:]) or "Дай три факта за езика Python."

    for model in LOCAL_MODELS:
        print(f"\n--- {model} ---")
        if not ollama_has(model):
            print(f"(не е изтеглен — пропускам)\n  -> {pull_commands(model)}")
            continue
        text, input_tokens, output_tokens, seconds = ask_local(question, model)
        print(text)
        print_usage("ollama", model, input_tokens, output_tokens, seconds)

    print(
        "\nСравнете трите реда с usage — този от Задача 1 и двата отгоре:"
        "\nлатентност, качество на българския и цена. После попълнете реда си"
        "\nв общата таблица."
    )


if __name__ == "__main__":
    main()
