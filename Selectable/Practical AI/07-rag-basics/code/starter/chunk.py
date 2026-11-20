"""LOAD и SPLIT: четем документите и ги делим на части (chunks).

Задача 1: попълнете split_text. Останалото е дадено.

Делим по думи: всяка част има chunk_size думи, а съседните части се
припокриват с overlap думи. Така изречение на границата попада цяло поне в една
част. Подходът е наивен (реже на произволно място); по-умни начини, по изречения
или по смисъл, има в Тема 8.
"""

from dataclasses import dataclass
from pathlib import Path


@dataclass
class Document:
    source: str  # името на файла, за цитиране
    text: str


@dataclass
class Chunk:
    id: str  # напр. "support.md#2"
    source: str
    index: int
    text: str


def load_documents(folder: str = "corpus") -> list[Document]:
    """LOAD: чете .md и .txt файловете от папката. (дадено)"""
    return [
        Document(p.name, p.read_text(encoding="utf-8"))
        for p in sorted(Path(folder).glob("*"))
        if p.suffix in {".md", ".txt"}
    ]


def split_text(text: str, chunk_size: int = 40, overlap: int = 10) -> list[str]:
    """SPLIT: части от chunk_size думи, като всяка започва overlap думи преди
    края на предишната."""
    # TODO: 1) разделете текста на думи (text.split())
    #       2) всяка нова част започва chunk_size - overlap думи след предишната
    #       3) върнете списък от части, всяка с до chunk_size думи, слепени с " "
    words = text.split()
    chunks = []
    step = max(chunk_size - overlap, 1)
    for i in range(0, len(words), step):
        part = words[i : i + chunk_size]
        chunks.append(" ".join(part))
    return chunks


def chunk_documents(docs, chunk_size: int = 40, overlap: int = 10) -> list[Chunk]:
    """Прилага split_text към всеки документ и номерира частите. (дадено)"""
    chunks = []
    for doc in docs:
        for i, piece in enumerate(split_text(doc.text, chunk_size, overlap) or []):
            chunks.append(Chunk(f"{doc.source}#{i}", doc.source, i, piece))
    return chunks
