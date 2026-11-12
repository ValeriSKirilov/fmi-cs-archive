import react from "@vitejs/plugin-react";
import { defineConfig } from "vite";

// Dev proxy: заявките към /chat отиват към нашия FastAPI сървър (Част A).
// Така НЯМА CORS и ключът остава изцяло от Python страна.
export default defineConfig({
  plugins: [react()],
  server: {
    proxy: {
      "/chat": "http://127.0.0.1:8000",
    },
  },
});
