from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "test/files-to-match/arquivo2-0"
OUTPUT_DIR = ROOT / "benchmarks/inputs"
SCALES = (1000, 100000, 200000)
CHUNK_SIZE = 1024 * 1024

def expand_file(contents: bytes, repetitions: int, output_path: Path):
    if not contents:
        raise ValueError(f"Arquivo vazio: {SOURCE}")
    copies_per_chunk = max(1, CHUNK_SIZE // len(contents))
    chunk = contents * copies_per_chunk

    with output_path.open("wb") as output:
        while repetitions >= copies_per_chunk:
            output.write(chunk)
            repetitions -= copies_per_chunk
        if repetitions:
            output.write(contents * repetitions)

def main():
    contents = SOURCE.read_bytes()
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    for scale in SCALES:
        output_path = OUTPUT_DIR / f"arquivo2-{scale}x"
        expected_size = len(contents) * scale

        if output_path.exists() and output_path.stat().st_size == expected_size:
            print(f"{output_path} ja existe.")
            continue

        expand_file(contents, scale, output_path)
        print(f"{output_path} gerado")

if __name__ == "__main__":
    main()
