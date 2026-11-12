// Подход 2: "useChat под капака" — малък React hook, който прави точно това, което
// Vercel AI SDK крие: fetch + ReadableStream reader + акумулиране на делти +
// optimistic append + отмяна. Говори с нашия FastAPI SSE backend от Част A.

import { useCallback, useRef, useState } from "react";

export type Msg = { id: string; role: "user" | "assistant"; text: string };
export type Status = "ready" | "streaming" | "error";

export function useChatStream(endpoint = "/chat") {
  const [messages, setMessages] = useState<Msg[]>([]);
  const [status, setStatus] = useState<Status>("ready");
  const controllerRef = useRef<AbortController | null>(null);

  const send = useCallback(
    async (text: string, provider = "ollama") => {
      const user: Msg = { id: crypto.randomUUID(), role: "user", text };
      const bot: Msg = { id: crypto.randomUUID(), role: "assistant", text: "" };
      setMessages((m) => [...m, user, bot]); // optimistic: показва се веднага
      setStatus("streaming");

      const controller = new AbortController();
      controllerRef.current = controller;
      try {
        const res = await fetch(
          `${endpoint}?q=${encodeURIComponent(text)}&provider=${provider}`,
          { signal: controller.signal }
        );
        const reader = res.body!.getReader();
        const dec = new TextDecoder();
        let buffer = "";

        for (;;) {
          const { value, done } = await reader.read(); // pull-based backpressure
          if (done) break;
          buffer += dec.decode(value, { stream: true });
          const events = buffer.split("\n\n"); // SSE: празен ред разделя събитията
          buffer = events.pop() ?? "";
          for (const evt of events) {
            const data = evt.replace(/^data: /, "");
            if (data === "[DONE]") {
              setStatus("ready");
              return;
            }
            if (data) {
              const delta = JSON.parse(data) as string;
              setMessages((m) =>
                m.map((x) => (x.id === bot.id ? { ...x, text: x.text + delta } : x))
              );
            }
          }
        }
        setStatus("ready");
      } catch (err) {
        setStatus((err as Error).name === "AbortError" ? "ready" : "error");
      }
    },
    [endpoint]
  );

  const stop = useCallback(() => controllerRef.current?.abort(), []);
  return { messages, status, send, stop };
}
