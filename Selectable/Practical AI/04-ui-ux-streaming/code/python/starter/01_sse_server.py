"""Задача 2: собствен SSE endpoint с FastAPI.

GET /chat?q=...&provider=... да стриймва отговора като Server-Sent Events.
Дадено е всичко останало: /chaos (за Задача 3), / (клиентът) и затварянето на потока.

Стартиране:
    python starter/01_sse_server.py       # после: curl -N --get --data-urlencode "q=Здравей" http://127.0.0.1:8000/chat
"""

import asyncio
import json
import sys
from pathlib import Path
from typing import Literal

import uvicorn
from dotenv import load_dotenv
from fastapi import FastAPI
from fastapi.responses import FileResponse, StreamingResponse

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))  # llm_stream.py
from llm_stream import STREAMERS, stream_ollama_sync

load_dotenv()
app = FastAPI()
HERE = Path(__file__).parent
NO_BUFFER = {"X-Accel-Buffering": "no"}  # без буфериране от прокси (nginx)

CHAOS_TEXT = ["Здравей", ", ", "свят!", "\nНов ред", " и ", "кирилица: ", "щ, ю, я.", " Готово."]


@app.get("/chaos")
async def chaos(size: int = 3):
    """ДАДЕНО (без модел): същия SSE поток, но нарязан на парчета от `size` байта.

    Мрежата може да направи това по всяко време; тук го правим нарочно и възпроизводимо, за да
    видите двата капана от лекцията в браузъра: събитие, разцепено наполовина,
    и буква на кирилица, разцепена между две парчета.
    """
    wire = "".join(f"data: {json.dumps(t, ensure_ascii=False)}\n\n" for t in CHAOS_TEXT) + "data: [DONE]\n\n"
    data = wire.encode("utf-8")

    async def pieces():
        for i in range(0, len(data), size):
            yield data[i: i + size]
            await asyncio.sleep(0.02)

    return StreamingResponse(pieces(), media_type="text/event-stream", headers=NO_BUFFER)


async def sse_stream(prompt: str, provider: str):
    sent = 0
    try:
        async for delta in STREAMERS[provider](prompt):
            if delta:
                yield f"data: {json.dumps(delta, ensure_ascii=False)}\n\n"
                sent += 1
        yield "data: [DONE]\n\n"
    except Exception as e:  # дадено: грешката се вижда в чата, а не като празен отговор
        yield f"data: {json.dumps(f'[грешка на сървъра: {e}]', ensure_ascii=False)}\n\n"
    finally:
        # Дадено: изпълнява се и когато браузърът прекъсне връзката (Задача 4).
        print(f"[сървър] потокът е затворен след {sent} делти", flush=True)


@app.get("/chat")
def chat(q: str, provider: Literal["ollama", "openai", "anthropic"] = "ollama"):
    return StreamingResponse(sse_stream(q, provider),
                             media_type="text/event-stream", headers=NO_BUFFER)


def sync_sse_stream(prompt: str):
    """ДАДЕНО (Задача 4): синхронната версия от слайдовете, за сравнение с бутона „Спри“."""
    sent = 0
    try:
        for delta in stream_ollama_sync(prompt):
            if delta:
                yield f"data: {json.dumps(delta, ensure_ascii=False)}\n\n"
                sent += 1
                if sent % 10 == 0:
                    print(f"[сървър/sync] още генерирам… {sent} делти", flush=True)
        yield "data: [DONE]\n\n"
    finally:
        print(f"[сървър/sync] потокът е затворен след {sent} делти", flush=True)


@app.get("/chat_sync")
def chat_sync(q: str):
    return StreamingResponse(sync_sse_stream(q), media_type="text/event-stream", headers=NO_BUFFER)


@app.get("/")
def index():
    return FileResponse(HERE / "client.html")


@app.get("/naive")
def naive():
    return FileResponse(HERE.parent / "solutions" / "client_naive.html")  # готов наивен клиент


if __name__ == "__main__":
    # Стартираме програмно (името на файла започва с цифра и не може да се импортира като модул).
    uvicorn.run(app, host="127.0.0.1", port=8000)
