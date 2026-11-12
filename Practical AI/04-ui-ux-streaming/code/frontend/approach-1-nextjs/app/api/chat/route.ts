// Подход 1: Next.js route handler — Vercel AI SDK streamText → SSE.
// Frontend и backend в ЕДИН проект: ключът стои сървърно, без CORS.
// Бонус (Generative UI): инструментът "weather" → React компонент на клиента.

import { chatModel } from "./model";
import { convertToModelMessages, stepCountIs, streamText, tool, type UIMessage } from "ai";
import { z } from "zod";

export const maxDuration = 30;

export async function POST(req: Request) {
  const { messages }: { messages: UIMessage[] } = await req.json();

  const result = streamText({
    model: chatModel,
    messages: convertToModelMessages(messages),
    tools: {
      weather: tool({
        description: "Върни текущото време за даден град.",
        inputSchema: z.object({ city: z.string() }),
        execute: async ({ city }) => ({ city, tempC: 21, condition: "слънчево" }),
      }),
    },
    stopWhen: stepCountIs(5),
  });

  return result.toUIMessageStreamResponse();
}
