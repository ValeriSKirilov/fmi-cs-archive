import react from "@vitejs/plugin-react";
import { defineConfig } from "vite";

// Плъгинът е задължителен: без него Vite ползва „класическия“ JSX преобразувател
// (React.createElement) и всеки .tsx файл иска `import React` — оттам идва
// „React is not defined“. Плъгинът включва автоматичния JSX runtime.
//
// За разлика от Подход 2 тук НЯМА proxy: сървърът е на друг origin
// (localhost:8787), затова Hono включва CORS. Точно това е цената на
// „два процеса“ — сравнете двата vite.config.ts.
export default defineConfig({
  plugins: [react()],
});
