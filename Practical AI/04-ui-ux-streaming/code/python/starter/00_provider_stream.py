"""Задача 1: стрийминг в терминала и измерване на TTFT.

Един и същ въпрос минава по два пътя:

  --no-stream   дадено: чакаме ЦЕЛИЯ отговор и чак тогава го печатаме (spinner).
  (по подразб.) вашето: печатаме токен по токен и мерим Time To First Token.

Общото време е същото; променя се колко бързо потребителят вижда нещо.

Стартиране:
    python starter/00_provider_stream.py "Какво е стрийминг?"
    python starter/00_provider_stream.py --no-stream "Какво е стрийминг?"

Изход СЛЕД TODO-тата:
    Стриймингът е ...
    [ollama: TTFT=1.20s общо=7.50s]
"""

import argparse
import asyncio
import sys
import time
from pathlib import Path

from dotenv import load_dotenv

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))  # llm_stream.py
from llm_stream import MODELS, STREAMERS, SetupError, complete


async def main() -> None:
    load_dotenv()
    parser = argparse.ArgumentParser()
    parser.add_argument("--provider", choices=list(MODELS), default="ollama")
    parser.add_argument("--no-stream", action="store_true")
    parser.add_argument("prompt", nargs="+")
    args = parser.parse_args()
    prompt = " ".join(args.prompt)

    start = time.perf_counter()
    if args.no_stream:
        print("Чакам…", flush=True)
        print(await complete(args.provider, prompt))
        print(f"\n[{args.provider}: без стрийминг, първият знак се вижда след {time.perf_counter() - start:.2f}s]")
        return

    ttft = None
    async for delta in STREAMERS[args.provider](prompt):
        if delta and ttft is None:
            ttft = time.perf_counter() - start
        print(delta, end="", flush=True)

    ttft_str = f"{ttft:.2f}" if ttft is not None else "n/a"
    total = time.perf_counter() - start
    print(f"\n\n[{args.provider}: TTFT={ttft_str}s общо={total:.2f}s]")


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except SetupError as e:
        sys.exit(str(e))
