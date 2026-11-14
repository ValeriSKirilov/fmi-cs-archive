// Избор на доставчик за Vercel AI SDK — локален по подразбиране.
//
// PROVIDER=ollama (по подразбиране) | anthropic
//
// Защо Ollama минава през createOpenAI: Ollama излага OpenAI-СЪВМЕСТИМ endpoint
// на /v1, така че същият клиент работи само със сменен baseURL. Това е добър
// пример как де факто стандарт спестява цял адаптер — сравнете с Тема 2, където
// разликите между доставчиците трябваше да се капсулират на ръка.

import { createAnthropic } from "@ai-sdk/anthropic";
import { createOpenAI } from "@ai-sdk/openai";

const PROVIDER = process.env.PROVIDER ?? "ollama";

function anthropicModel() {
  // Поддържаме и двата вида идентификация (виж Тема 1):
  //   ANTHROPIC_API_KEY   -> header x-api-key
  //   ANTHROPIC_AUTH_TOKEN -> header Authorization: Bearer
  const token = process.env.ANTHROPIC_AUTH_TOKEN;
  const client = token
    ? createAnthropic({ apiKey: "unused", headers: { "x-api-key": "", authorization: `Bearer ${token}` } })
    : createAnthropic(); // чете ANTHROPIC_API_KEY от средата
  return client(process.env.ANTHROPIC_MODEL ?? "claude-haiku-4-5");
}

function ollamaModel() {
  const ollama = createOpenAI({
    baseURL: process.env.OLLAMA_URL ?? "http://localhost:11434/v1",
    apiKey: "ollama", // Ollama не проверява ключ, но SDK-то иска низ
  });
  // .chat(...) — а НЕ ollama(...): @ai-sdk/openai v2 по подразбиране ползва
  // новото Responses API на OpenAI, което Ollama не имплементира. Ollama излага
  // само /v1/chat/completions, затова избираме изрично chat варианта.
  // llama3.2 (3B), а не :1b — 1B се справя зле с български и с tool calling.
  return ollama.chat(process.env.OLLAMA_MODEL ?? "llama3.2");
}

export const chatModel = PROVIDER === "anthropic" ? anthropicModel() : ollamaModel();
export const providerName = PROVIDER;
