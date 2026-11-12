// Generative UI компонент: рендира резултата от инструмента "weather"
// вместо суров текст. Данните идват от tool-частта в потока.

export function WeatherCard({
  city,
  tempC,
  condition,
}: {
  city: string;
  tempC: number;
  condition: string;
}) {
  return (
    <div
      style={{
        border: "1px solid #cbd5e1",
        borderRadius: 8,
        padding: "0.75rem 1rem",
        margin: "0.5rem 0",
        background: "#f5f8ff",
        display: "inline-block",
      }}
    >
      <strong>{city}</strong>
      <div>
        {tempC}°C · {condition}
      </div>
    </div>
  );
}
