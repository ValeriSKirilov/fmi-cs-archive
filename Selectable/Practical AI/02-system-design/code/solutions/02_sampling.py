"""Задача 2 — параметри за семплиране: temperature и max_tokens.

Параметрите не са забити в кода, а флагове. Два експеримента:

  1) --sweep: един и същ промпт по 3 пъти при temperature 0.0 и 1.0.
     Ниската дава почти един и същ отговор (стабилност), високата — различни
     (разнообразие).
  2) --max-tokens 15: таванът на ИЗХОДА реже отговора по средата. Трябва да
     го засечем и да предупредим, вместо да покажем половин изречение.

Стартиране:
    python solutions/02_sampling.py --sweep "Измисли име за кафене."
    python solutions/02_sampling.py --max-tokens 15 "Разкажи за София."
    python solutions/02_sampling.py --sweep --seed 42 "Измисли име за кафене."
    python solutions/02_sampling.py --stop . "Разкажи за София."

Изход (bggpt:4b; вашите имена ще са други):
    === temperature=0.0 ===
     • "Кафе Арт"
     • "Кафе Арт"
     • "Кафе Арт"
    === temperature=1.0 ===
     • "Ароматна кафеджийница"
     • "Златен лист" или "Кафе с аромат"
     • Кафе "Релакс" или "Арома"

    $ python solutions/02_sampling.py --max-tokens 15 "Разкажи за София."
    [!] отговорът е отрязан от max_tokens — увеличете --max-tokens
    София е столицата на България и най-големият град
       [ollama/bggpt:4b  вход=28 изход=15  ·  6.37s]
"""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))  # за `import llm`
from llm import Reply, chat, cli, print_usage

SYSTEM = "Отговаряй кратко, на български."


def generate(prompt: str, *, temperature: float | None, top_p: float | None,
             max_tokens: int, provider: str, model: str | None,
             seed: int | None = None, stop: list[str] | None = None) -> Reply:
    reply = chat(SYSTEM, [{"role": "user", "content": prompt}],
                 temperature=temperature, top_p=top_p, max_tokens=max_tokens,
                 seed=seed, stop=stop, provider=provider, model=model)
    if reply.truncated:  # отговорът е спрял заради тавана, а не защото е завършен
        print("[!] отговорът е отрязан от max_tokens — увеличете --max-tokens")
    return reply


def sweep(prompt: str, max_tokens: int, provider: str, model: str | None,
          seed: int | None) -> None:
    """Един и същ промпт по 3 пъти при ниска и при висока temperature."""
    for t in (0.0, 1.0):
        print(f"\n=== temperature={t} ===")
        for _ in range(3):
            reply = generate(prompt, temperature=t, top_p=None, max_tokens=max_tokens,
                             provider=provider, model=model, seed=seed)
            print(" •", reply.text.replace("\n", " "))
    print("\nНиската temperature повтаря един и същ отговор, високата — разнообразява.")


def main() -> None:
    parser = cli("Конфигурируеми параметри за семплиране.")
    parser.add_argument("--temperature", type=float, default=0.7)
    parser.add_argument("--top-p", type=float, default=None,
                        help="nucleus sampling; задайте ЕДНО от --temperature/--top-p")
    parser.add_argument("--max-tokens", type=int, default=256)
    parser.add_argument("--sweep", action="store_true",
                        help="сравни temperature 0.0 и 1.0, по 3 пъти")
    parser.add_argument("--seed", type=int, default=None,
                        help="фиксира случайността: същият seed -> същият избор")
    parser.add_argument("--stop", action="append", default=None,
                        help="спри генерирането при този низ (напр. --stop .)")
    parser.add_argument("prompt", nargs="*")
    args = parser.parse_args()
    prompt = " ".join(args.prompt) or "Измисли име за кафене."

    if args.sweep:
        sweep(prompt, args.max_tokens, args.provider, args.model, args.seed)
        return

    # Настройваме ЕДНО от двете: при зададен --top-p temperature не се подава.
    temperature = None if args.top_p is not None else args.temperature
    reply = generate(prompt, temperature=temperature, top_p=args.top_p,
                     max_tokens=args.max_tokens, provider=args.provider, model=args.model,
                     seed=args.seed, stop=args.stop)
    print(reply.text)
    print_usage(reply)


if __name__ == "__main__":
    main()
