// Подход 3: отделен лек Node сървър (Hono), който пази ключа и ползва Vercel AI SDK.
// Клиентът (Vite React) сочи useChat към този endpoint. Понеже е различен origin
// от Vite dev сървъра, включваме CORS.

// Node не чете .env сам (за разлика от Next.js) — зареждаме го явно.
import "dotenv/config";

import { serve } from "@hono/node-server";
import { chatModel, providerName } from "./model";
import { convertToModelMessages, streamText, type UIMessage } from "ai";
import { Hono } from "hono";
import { cors } from "hono/cors";

const app = new Hono();
app.use("/api/*", cors()); // нужно, защото frontend и backend са различни origin

app.post("/api/chat", async (c) => {
  const { messages } = await c.req.json<{ messages: UIMessage[] }>();
  const result = streamText({
    model: chatModel,
    messages: convertToModelMessages(messages),
  });
  return result.toUIMessageStreamResponse();
});

serve({ fetch: app.fetch, port: 8787 });
console.log(`Node сървър на http://localhost:8787 (доставчик: ${providerName})`);
