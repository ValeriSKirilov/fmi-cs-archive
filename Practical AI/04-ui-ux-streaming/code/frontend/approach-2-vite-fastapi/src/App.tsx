// Подход 2: чист React (Vite) UI върху нашия FastAPI backend, без Vercel AI SDK.
// Цялото стрийминг състояние идва от собствения hook useChatStream.

import { useState } from "react";
import { useChatStream } from "./useChatStream";

export default function App() {
  const { messages, status, send, stop } = useChatStream("/chat");
  const [input, setInput] = useState("");
  const [provider, setProvider] = useState("ollama");

  return (
    <main style={{ maxWidth: 640, margin: "2rem auto", fontFamily: "system-ui", padding: "0 1rem" }}>
      <h1>AI чат — Подход 2 (Vite React + FastAPI)</h1>

      {messages.map((m) => (
        <div key={m.id} style={{ margin: "1rem 0" }}>
          <strong>{m.role === "user" ? "Вие" : "Асистент"}: </strong>
          <span>{m.text}</span>
          {m.role === "assistant" && !m.text && status === "streaming" && <em>…</em>}
        </div>
      ))}

      {status === "streaming" && <button onClick={stop}>Спри</button>}
      {status === "error" && <div style={{ color: "#b91c1c" }}>Възникна грешка.</div>}

      <form
        onSubmit={(e) => {
          e.preventDefault();
          if (!input.trim()) return;
          send(input, provider);
          setInput("");
        }}
        style={{ display: "flex", gap: "0.5rem", marginTop: "1rem" }}
      >
        <select value={provider} onChange={(e) => setProvider(e.target.value)}>
          <option value="openai">openai</option>
          <option value="anthropic">anthropic</option>
          <option value="ollama">ollama</option>
        </select>
        <input
          value={input}
          onChange={(e) => setInput(e.target.value)}
          placeholder="Попитайте нещо…"
          disabled={status === "streaming"}
          style={{ flex: 1, padding: "0.5rem" }}
        />
        <button type="submit" disabled={status === "streaming"}>Изпрати</button>
      </form>
    </main>
  );
}
