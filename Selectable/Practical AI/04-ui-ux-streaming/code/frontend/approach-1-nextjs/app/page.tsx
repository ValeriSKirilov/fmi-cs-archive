// Подход 1: клиент с useChat — цялото състояние на чата идва от SDK-то.
// optimistic append, typing индикатор, "Спри" (само при streaming), грешка +
// regenerate, и рендиране на message.parts (текст + Generative UI компонент).

"use client";

import { useChat } from "@ai-sdk/react";
import { useState } from "react";
import { WeatherCard } from "./weather-card";

export default function Chat() {
  const { messages, sendMessage, status, stop, error, regenerate } = useChat();
  const [input, setInput] = useState("");

  return (
    <main style={{ maxWidth: 640, margin: "2rem auto", fontFamily: "system-ui", padding: "0 1rem" }}>
      <h1>AI чат — Подход 1 (Next.js + AI SDK)</h1>

      {messages.map((m) => (
        <div key={m.id} style={{ margin: "1rem 0" }}>
          <strong>{m.role === "user" ? "Вие" : "Асистент"}: </strong>
          {m.parts.map((part, i) => {
            switch (part.type) {
              case "text":
                return <span key={i}>{part.text}</span>;
              case "tool-weather":
                if (part.state === "output-available") {
                  return <WeatherCard key={i} {...part.output} />;
                }
                return <div key={i}>Проверявам времето…</div>;
              default:
                return null;
            }
          })}
        </div>
      ))}

      {status === "submitted" && <div>…</div>}
      {status === "streaming" && <button onClick={stop}>Спри</button>}
      {error && (
        <div style={{ color: "#b91c1c" }}>
          Възникна грешка. <button onClick={() => regenerate()}>Опитай пак</button>
        </div>
      )}

      <form
        onSubmit={(e) => {
          e.preventDefault();
          if (!input.trim()) return;
          sendMessage({ text: input });
          setInput("");
        }}
        style={{ display: "flex", gap: "0.5rem", marginTop: "1rem" }}
      >
        <input
          value={input}
          onChange={(e) => setInput(e.target.value)}
          placeholder="Попитайте нещо… (напр. „Какво е времето в София?“)"
          disabled={status === "streaming" || status === "submitted"}
          style={{ flex: 1, padding: "0.5rem" }}
        />
        <button type="submit" disabled={status === "streaming" || status === "submitted"}>
          Изпрати
        </button>
      </form>
    </main>
  );
}
