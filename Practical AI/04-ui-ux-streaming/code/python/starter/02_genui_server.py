"""Задача 5: Generative UI без библиотека — сървърът (даден, не се пише).

Потокът вече не е само текст, а СЪБИТИЯ с тип, опростено подобие на частите (message.parts)
в Vercel AI SDK (там полето е toolCallId, а по мрежата идват chunk-ове):

    {"type": "tool-weather", "id": "w1", "state": "input-streaming"}
    {"type": "tool-weather", "id": "w1", "state": "input-available", "input": {"city": "Варна"}}
    {"type": "tool-weather", "id": "w1", "state": "output-available", "output": {...}}
    {"type": "text", "delta": "Облечете "}

ВНИМАНИЕ: „решението“ дали да се извика инструментът се взима с регулярен израз, а не
от модела, за да е упражнението възпроизводимо. Моделът само пише текста накрая.
Истинското tool calling (моделът избира) е в Тема 9.

Стартиране:
    python starter/02_genui_server.py       # после http://127.0.0.1:8001/
"""

import asyncio
import json
import re
import sys
from pathlib import Path

import uvicorn
from dotenv import load_dotenv
from fastapi import FastAPI
from fastapi.responses import FileResponse, StreamingResponse

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))  # llm_stream.py
from llm_stream import stream_ollama

load_dotenv()
app = FastAPI()
HERE = Path(__file__).parent

WEATHER = {
    "София": {"tempC": 21, "condition": "слънчево"},
    "Варна": {"tempC": 24, "condition": "облачно"},
    "Пловдив": {"tempC": 27, "condition": "слънчево"},
    "Бургас": {"tempC": 19, "condition": "дъждовно"},
}


def sse(event: dict) -> str:
    return f"data: {json.dumps(event, ensure_ascii=False)}\n\n"


def city_from(question: str) -> str | None:
    m = re.search(r"врем\w*\s+(?:в|във)\s+([\w-]+)", question, re.IGNORECASE)
    return m.group(1).capitalize() if m else None


async def events(question: str):
    city = city_from(question)
    prompt = question
    if city:
        tool = {"type": "tool-weather", "id": "w1"}
        yield sse({**tool, "state": "input-streaming"})  # аргументите още пристигат
        await asyncio.sleep(0.8)
        yield sse({**tool, "state": "input-available", "input": {"city": city}})
        await asyncio.sleep(1.0)  # „execute“ работи на сървъра
        if city in WEATHER:
            output = {"city": city, **WEATHER[city]}
            yield sse(
                {
                    **tool,
                    "state": "output-available",
                    "input": {"city": city},
                    "output": output,
                }
            )
            prompt = (
                f"Времето в {city} е {output['tempC']}°C, {output['condition']}. "
                "Дай един кратък съвет какво да облека."
            )
        else:
            yield sse(
                {
                    **tool,
                    "state": "output-error",
                    "errorText": f"Няма данни за „{city}“.",
                }
            )
            yield "data: [DONE]\n\n"
            return
    try:
        async for delta in stream_ollama(prompt):
            if delta:
                yield sse({"type": "text", "delta": delta})
    except Exception as e:
        yield sse({"type": "text", "delta": f"[грешка на сървъра: {e}]"})
    yield "data: [DONE]\n\n"


@app.get("/ask")
async def ask(q: str):
    return StreamingResponse(
        events(q), media_type="text/event-stream", headers={"X-Accel-Buffering": "no"}
    )


@app.get("/")
def index():
    return FileResponse(HERE / "genui.html")


if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8001)
