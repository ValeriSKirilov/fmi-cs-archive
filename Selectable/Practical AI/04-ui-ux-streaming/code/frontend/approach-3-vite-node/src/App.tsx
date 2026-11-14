// Подход 3: чист React (Vite) + useChat, сочещ към отделния Node сървър.
// Запазваме SDK ергономиката (parts, status, stop), но плащаме с втори процес + CORS.

import { useChat } from "@ai-sdk/react";
import { DefaultChatTransport } from "ai";
import { useState } from "react";

export default function App() {
  const { messages, sendMessage, status, stop, error, regenerate } = useChat({
    transport: new DefaultChatTransport({ api: "http://localhost:8787/api/chat" }),
  });
  const [input, setInput] = useState("");

  return (
    <main style={{ maxWidth: 640, margin: "2rem auto", fontFamily: "system-ui", padding: "0 1rem" }}>
      <h1>AI чат — Подход 3 (Vite React + Node/AI SDK)</h1>

      {messages.map((m) => (
        <div key={m.id} style={{ margin: "1rem 0" }}>
          <strong>{m.role === "user" ? "Вие" : "Асистент"}: </strong>
          {m.parts.map((part, i) =>
            part.type === "text" ? <span key={i}>{part.text}</span> : null
          )}
        </div>
      ))}

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
          placeholder="Попитайте нещо…"
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
