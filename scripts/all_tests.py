import argparse
import csv
import random
import statistics
import tempfile
from pathlib import Path

from assignment_benchmark import ROOT, compile_programs, fixture_pairs, run_program


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runs", type=int, default=10)
    args = parser.parse_args()
    if args.runs < 1:
        parser.error("--runs must be positive")

    rows = []
    summaries = []
    failures = 0
    order = random.Random(42)
    with tempfile.TemporaryDirectory(prefix="all-tests-") as directory:
        programs = compile_programs(Path(directory))
        for pattern, target in fixture_pairs():
            expected, _ = run_program(programs["reference"], pattern, target)
            case_rows = []
            for repetition in range(1, args.runs + 1):
                names = list(programs)
                order.shuffle(names)
                for name in names:
                    threads = 4 if name == "parallel" else 1
                    count, seconds = run_program(programs[name], pattern, target, threads)
                    matches = count == expected
                    failures += not matches
                    case_rows.append({
                        "pattern": pattern.name,
                        "target": target.name,
                        "program": name,
                        "threads": threads,
                        "repetition": repetition,
                        "seconds": seconds,
                        "occurrences": count,
                        "reference_count": expected,
                        "status": "PASS" if matches else "FAIL",
                    })
            rows.extend(case_rows)
            for name in programs:
                observations = [row for row in case_rows if row["program"] == name]
                times = [row["seconds"] for row in observations]
                mean = statistics.mean(times)
                sd = statistics.stdev(times) if args.runs > 1 else 0.0
                status = "PASS" if all(row["status"] == "PASS" for row in observations) else "FAIL"
                summaries.append(
                    f"| {pattern.name} / {target.name} | {name} | "
                    f"{observations[0]['threads']} | {mean:.6f} | {sd:.6f} | {expected} | {status} |"
                )
            print(f"Completed {pattern.name} / {target.name}", flush=True)

    output_dir = ROOT / "benchmarks/all_tests"
    output_dir.mkdir(parents=True, exist_ok=True)
    with (output_dir / "results.csv").open("w", newline="") as output:
        writer = csv.DictWriter(output, fieldnames=list(rows[0]))
        writer.writeheader()
        writer.writerows(rows)

    report = [
        "# All paired test fixtures",
        "",
        f"{args.runs} runs per program and fixture.",
        "",
        "Raw observations: [results.csv](results.csv).",
        "",
        "| Fixture pair | Program | Threads | Mean seconds | SD seconds | Reference count | Status |",
        "| --- | --- | ---: | ---: | ---: | ---: | --- |",
        *summaries,
        "",
        f"Count mismatches: {failures}.",
    ]
    (output_dir / "results.md").write_text("\n".join(report) + "\n")
    print(f"Wrote results to {output_dir}; mismatches: {failures}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
