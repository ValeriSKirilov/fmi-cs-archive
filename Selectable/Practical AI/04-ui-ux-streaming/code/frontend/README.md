# Част B — три начина за стрийминг frontend (разглеждане, не задача)

Тук **не** решаваме задача с `TODO`. Разглеждаме **три работещи решения** за един и
същ проблем — стрийминг chat интерфейс — и сравняваме кога кое е уместно. Целта е да
разберете компромисите, а не да наизустите един стек.

## Изходната точка: защо изобщо ни трябва backend?

> **API ключът НЕ може да стои в браузъра.** Ако React приложението извика OpenAI
> директно, ключът изтича към всеки потребител. Затова **при всички положения** ни
> трябва сървър, който пази ключа и стриймва към клиента.

Оттук реалният въпрос не е „React или Next“, а **къде да седи сървърът** и **колко
да си пишем сами** срещу колко да поеме библиотека (Vercel AI SDK). Трите подхода са
трите отговора.

## Сравнение накратко

| | Подход 1 — Next.js | Подход 2 — Vite React + FastAPI | Подход 3 — Vite React + Node |
|---|---|---|---|
| Vercel AI SDK | да (`useChat`) | **не** — свой hook | да (`useChat`) |
| Backend | Next route (същ проект) | нашият FastAPI (Част A) | отделен Node (Hono) |
| Брой процеси | **1** | 2 (+ dev proxy) | 2 (+ CORS) |
| CORS | няма | няма (proxy) | **да** |
| Ключът | сървърна страна | сървърна страна (Python) | сървърна страна (Node) |
| Generative UI | лесно (`parts`) | ръчно (свой протокол) | лесно (`parts`) |
| „Под капака“ | скрито в SDK | **видимо** (сам пишеш hook-а) | скрито в SDK |
| Най-подходящ за | продукт с TS full-stack | екип на Python + лек React UI | съществуващ Node backend |

**Как да го четете:** „по-лесно“ зависи от целта. За да учите *SDK абстракцията*
(Въпрос 13: Generative UI) — Подход 1 е с най-малко триене. За да видите *какво прави
useChat под капака* и да останете на Python backend — Подход 2. Подход 3 показва, че
запазването на SDK извън Next струва отделен сървър + CORS.

---

## Подход 1 — Next.js + Vercel AI SDK  (`approach-1-nextjs/`)

Frontend и backend в **един** проект; route handler-ът пази ключа. Стандартният начин за
ползване на SDK-то — цялата документация е около него.

```ts
// app/api/chat/route.ts — сървър
const result = streamText({ model: chatModel,   // Ollama llama3.2 или Anthropic, по PROVIDER
  messages: convertToModelMessages(messages) });
return result.toUIMessageStreamResponse();
```
```tsx
// app/page.tsx — клиент
const { messages, sendMessage, status, stop, error, regenerate } = useChat();
```

**Плюсове:** нула конфигурация за връзката, без CORS, `useChat` дава optimistic UI,
stop, грешки и Generative UI (`parts`) наготово. **Минуси:** носи Next концепции
(App Router, server/client компоненти), които може да са нови за студентите.
**Кога:** когато продуктът е TypeScript full-stack и искате SDK ергономиката.

---

## Подход 2 — Vite React върху нашия FastAPI  (`approach-2-vite-fastapi/`)

Без Next и **без Vercel AI SDK**. Vite React frontend говори с вече готовия FastAPI
SSE backend от Част A. Пишем малък hook `useChatStream`, който прави точно това, което
`useChat` крие: fetch + reader + акумулиране на делти + optimistic + stop.

```ts
// src/useChatStream.ts (същината) — "useChat под капака"
const res = await fetch(`${endpoint}?q=${q}`, { signal: controller.signal });
const reader = res.body!.getReader();
for (;;) {
  const { value, done } = await reader.read();      // pull-based backpressure
  if (done) break;
  // ... split по "\n\n", махни "data: ", [DONE] => край, иначе append делтата
}
```

**Плюсове:** най-последователно с философията „под капака“; ключът остава от Python
страна; преизползва Част A; студентите виждат целия механизъм. **Минуси:** пишете
сами управлението на състоянието; Generative UI изисква **свой** event протокол
(което е точно каквото SDK-то стандартизира — добра илюстрация защо съществува).
**Кога:** екип на Python с лек React UI; учебна прозрачност.

> Свързването е през Vite dev proxy (`/chat` → `http://127.0.0.1:8000`), затова
> **няма CORS**. Първо пуснете FastAPI сървъра от `../python/`.

---

## Подход 3 — Vite React + малък Node сървър с AI SDK  (`approach-3-vite-node/`)

Запазва `useChat`/Generative UI, но извън Next: отделен лек Node сървър (Hono), който
държи ключа и ползва `streamText`. Клиентът сочи `useChat` към него през `transport`.

```ts
// server.ts — Hono, пази ключа
app.post("/api/chat", async (c) => {
  const { messages } = await c.req.json();
  const result = streamText({ model: chatModel,   // Ollama llama3.2 или Anthropic, по PROVIDER
    messages: convertToModelMessages(messages) });
  return result.toUIMessageStreamResponse();
});
```
```tsx
// src/App.tsx — клиентът сочи към отделния сървър
const { messages, sendMessage, status } = useChat({
  transport: new DefaultChatTransport({ api: "http://localhost:8787/api/chat" }),
});
```

**Плюсове:** SDK ергономиката без Next; чист Vite frontend. **Минуси:** два проекта,
**CORS**, две конфигурации — обикновено най-много движещи се части. **Кога:** вече
имате Node backend и искате да добавите SDK-базиран чат към него.

---

## Пускане

**Подход 1** (един процес):
```bash
cd approach-1-nextjs
npm install        # или копирайте файловете в свеж `npx create-next-app`
cp .env.example .env.local   # PROVIDER=ollama (нужен е `ollama pull llama3.2`) или anthropic
npm run dev        # http://localhost:3000
```

**Подход 2** (FastAPI + Vite, dev proxy — ключът е при Python):
```bash
# терминал 1: backend от Част A
cd ../python && python solutions/01_sse_server.py     # http://127.0.0.1:8000
# терминал 2: frontend
cd approach-2-vite-fastapi && npm install && npm run dev   # http://localhost:5173
```

**Подход 3** (Node сървър + Vite, два процеса + CORS):
```bash
cd approach-3-vite-node
npm install
cp .env.example .env          # PROVIDER=ollama (нужен е `ollama pull llama3.2`) или anthropic
npm run server                # терминал 1: http://localhost:8787
npm run dev                   # терминал 2: http://localhost:5173
```

> Файловете са **работещи референтни примери** за сравнение. Версиите на Vercel AI
> SDK се менят: тук е закована v5 (`"ai": "^5.0.0"`), а актуалната е v7. От v6 нататък
> напр. `convertToModelMessages` се извиква с `await`. Сверявайте се с документацията.

## Извод

- Искате да учите **SDK абстракцията** и Generative UI с най-малко триене → **Подход 1**.
- Искате **прозрачност** („какво прави useChat“) и/или **Python backend** → **Подход 2**.
- Имате **съществуващ Node сървър** и държите на SDK-то → **Подход 3**.

И трите пазят ключа на сървъра и стриймват по SSE, но с различен протокол: при 1 и 3 е
UI message stream на SDK-то (типизирани chunk-ове като `{"type":"text-delta",…}`), при 2 е
нашият `data: "<делта>"` + `[DONE]`. Механизмът е същият, който видяхме суров в Част A.
