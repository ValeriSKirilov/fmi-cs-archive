"""Задача 1: стрийминг в терминала и измерване на TTFT (решение).

Стартиране:
    python solutions/00_provider_stream.py "Какво е стрийминг?"
    python solutions/00_provider_stream.py --no-stream "Какво е стрийминг?"
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
            ttft = time.perf_counter() - start  # първата непразна делта = TTFT
        print(delta, end="", flush=True)

    total = time.perf_counter() - start
    ttft_str = f"{ttft:.2f}s" if ttft is not None else "n/a"
    print(f"\n\n[{args.provider}: TTFT={ttft_str} общо={total:.2f}s]")


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except SetupError as e:
        sys.exit(str(e))
