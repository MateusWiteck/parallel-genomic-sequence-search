import argparse
import csv
import os
import re
import shlex
import statistics
import subprocess
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PATTERN_DIR = ROOT / "test/pattern-files"
TARGET_DIR = ROOT / "test/files-to-match"
INPUT_DIR = ROOT / "benchmarks/inputs"
OUTPUT_DIR = ROOT / "benchmarks"
COUNT_PATTERN = re.compile(r"Total de ocorrencias\s*=\s*(\d+)")
SIZES = (1, 1000, 100000, 200000)
THREAD_COUNTS = (1, 2, 4, 8)

def compile_programs(build_dir: Path) -> dict[str, Path]:
    compiler = shlex.split(os.environ.get("CC", "gcc"))
    cflags = shlex.split(os.environ.get("CFLAGS", "-O3 -Wall -Wextra"))
    ompflags = shlex.split(os.environ.get("OMPFLAGS", "-fopenmp"))
    sources = {
        "reference": ROOT / "reference/busca-cadeia.c",
        "parallel": ROOT / "src/busca-paralela.c",
        "sequential-optimized": ROOT / "src/busca-cadeia-otimizado.c",
    }

    executables = {}
    for name, source in sources.items():
        flags = [] if name == "reference" else ompflags
        executable = build_dir / name
        subprocess.run(compiler + cflags + flags + [str(source), "-o", str(executable)], check=True)
        executables[name] = executable
    return executables


def run_program(executable: Path, pattern: Path, target: Path, threads: int = 1) -> tuple[int, float]:
    environment = os.environ.copy()
    environment["OMP_NUM_THREADS"] = str(threads)
    environment["OMP_DYNAMIC"] = "FALSE"

    start = time.perf_counter()
    result = subprocess.run(
        [str(executable), str(pattern), str(target)],
        check=True,
        capture_output=True,
        text=True,
        env=environment,
    )
    elapsed = time.perf_counter() - start

    match = COUNT_PATTERN.search(result.stdout)
    if match is None:
        raise RuntimeError(f"No occurrence count found in output from {executable}")
    return int(match.group(1)), elapsed


def fixture_pairs() -> list[tuple[Path, Path]]:
    """Find pattern files that have a matching target file."""
    pairs = []
    for pattern in sorted(PATTERN_DIR.glob("arquivo1*")):
        suffix = pattern.name[len("arquivo1"):]
        target_name = "arquivo2-0" if not suffix else f"arquivo2{suffix}"
        target = TARGET_DIR / target_name

        if not target.exists() and suffix.startswith("-"):
            target = TARGET_DIR / f"arquivo2{suffix[1:]}"
        if target.exists():
            pairs.append((pattern, target))

    if not pairs:
        raise RuntimeError("No paired arquivo1/arquivo2 fixtures found")
    return pairs


def check_counts(programs: dict[str, Path], pattern: Path, target: Path) -> int:
    """Run all versions and fail if their occurrence counts differ."""
    counts = {
        "reference": run_program(programs["reference"], pattern, target)[0],
        "parallel": run_program(programs["parallel"], pattern, target, threads=4)[0],
        "sequential-optimized": run_program(programs["sequential-optimized"], pattern, target)[0],
    }
    if len(set(counts.values())) != 1:
        raise RuntimeError(f"Count mismatch for {pattern.name} / {target.name}: {counts}")
    return counts["reference"]


def check_fixture_counts(programs: dict[str, Path]) -> None:
    for pattern, target in fixture_pairs():
        count = check_counts(programs, pattern, target)
        print(f"PASS {pattern.name} / {target.name}: {count}")


def target_for_size(size: int) -> Path:
    if size == 1:
        return TARGET_DIR / "arquivo2-0"
    return INPUT_DIR / f"arquivo2-{size}x"


def check_scaled_counts(programs: dict[str, Path], pattern: Path) -> None:
    for size in SIZES:
        target = target_for_size(size)
        if not target.exists():
            raise RuntimeError(f"Missing {target}; run `make benchmark-inputs` first")
        check_counts(programs, pattern, target)


def benchmark_configurations():
    """Yield (group, size, program, thread count) rows to time."""
    for size in SIZES:
        for program, threads in (("reference", 1), ("parallel", 4), ("sequential-optimized", 1)):
            yield f"size-{size}x", size, program, threads

    for threads in THREAD_COUNTS:
        yield "threads-200000x", 200000, "parallel", threads


def collect_timings(programs: dict[str, Path], pattern: Path, runs: int) -> list[dict[str, object]]:
    rows = []
    for group, size, program, threads in benchmark_configurations():
        executable = programs[program]
        target = target_for_size(size)

        for repetition in range(1, runs + 1):
            count, seconds = run_program(executable, pattern, target, threads)
            rows.append({
                "group": group,
                "scale": size,
                "program": program,
                "threads": threads,
                "repetition": repetition,
                "seconds": f"{seconds:.9f}",
                "occurrences": count,
            })
        print(f"BENCH {group}: {program}, {threads} thread(s), {runs} runs")
    return rows


def write_results(rows: list[dict[str, object]], runs: int) -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    csv_path = OUTPUT_DIR / "assignment-scaling.csv"
    summary_path = OUTPUT_DIR / "assignment-scaling.md"

    with csv_path.open("w", newline="") as output:
        writer = csv.DictWriter(output, fieldnames=list(rows[0]))
        writer.writeheader()
        writer.writerows(rows)

    groups = {}
    for row in rows:
        key = (row["group"], row["program"], row["threads"])
        groups.setdefault(key, []).append(row)

    summary = [
        "# Assignment scaling benchmark",
        "",
        f"Runs per configuration: {runs}. Raw observations: [{csv_path.name}]({csv_path.name}).",
        "",
        "| Configuration | Program | Threads | Mean seconds | SD seconds | Occurrences |",
        "| --- | --- | ---: | ---: | ---: | ---: |",
    ]
    for (group, program, threads), observations in groups.items():
        times = [float(row["seconds"]) for row in observations]
        mean = statistics.mean(times)
        deviation = statistics.stdev(times) if len(times) > 1 else 0.0
        count = observations[0]["occurrences"]
        summary.append(
            f"| {group} | {program} | {threads} | {mean:.6f} | {deviation:.6f} | {count} |"
        )

    summary_path.write_text("\n".join(summary) + "\n")
    print(f"Wrote benchmark data to {csv_path} and {summary_path}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--correctness-only", action="store_true")
    parser.add_argument("--runs", type=int, default=10,
                        help="timed repetitions per configuration (default: 10)")
    args = parser.parse_args()
    if args.runs < 1:
        parser.error("--runs must be positive")

    pattern = PATTERN_DIR / "arquivo1"
    if not pattern.exists():
        raise RuntimeError(f"Missing default pattern file: {pattern}")

    with tempfile.TemporaryDirectory(prefix="assignment-benchmark-") as temp_dir:
        programs = compile_programs(Path(temp_dir))
        check_fixture_counts(programs)
        if args.correctness_only:
            print("All fixture counts match the supplied reference.")
            return 0

        check_scaled_counts(programs, pattern)
        rows = collect_timings(programs, pattern, args.runs)
        write_results(rows, args.runs)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
