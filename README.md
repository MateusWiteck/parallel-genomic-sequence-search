# Parallel Genomic Sequence Search

High-performance genomic sequence search in **C** using **OpenMP**. Course project for **PCS3868 — Sistemas de Computação de Alto Desempenho (USP, 2026)**.

## Overview

The program counts occurrences of a genomic sequence (a sequence of nucleotide-base strings) from `arquivo1` within the genomic sequences in `arquivo2`. Each genomic sequence is stored as a line; the final line may omit its newline character. **Overlapping occurrences must not be counted.**

The goal is to improve execution time through algorithmic optimizations and parallel processing, while preserving the result of the provided reference implementation (`busca-cadeia.c`).

## Required implementations

| File | Purpose |
| --- | --- |
| `src/baseline_sequential.c` | Original sequential reference (`busca-cadeia.c`), retained for comparison |
| `src/baseline_parallel.c` | OpenMP parallelization of the supplied reference |
| `src/optimized_sequential.c` | New optimized sequential implementation |
| `src/optimized_parallel.c` | OpenMP parallelization of the optimized implementation |

> These filenames and the directory structure are proposed conventions, not filenames mandated by the assignment. Implementations are not included in this initial repository scaffold.

## Repository structure

```text
parallel-genomic-sequence-search/
├── README.md
├── Makefile
├── src/
│   └── ... implementation sources ...
├── test/
│   ├── pattern-files/
│   └── files-to-match/
├── reference/
│   ├── busca-cadeia.c
│   └── mede_time.h
├── scripts/
│   └── aumentar-arquivo
├── docs/
│   ├── Enunciado do Projeto 2026.pdf
│   └── PROJETO_TESTES_ADICIONAIS.pdf
├── benchmarks/
│   ├── results/
│   └── plots/
└── report/
```

The supplied reference implementation and timer header are kept under `reference/`, the input-generation utility is under `scripts/`, and assignment PDFs are under `docs/`. Add implementation sources under `src/`. Keep paired test inputs under `test/` with matching filename suffixes (for example, `arquivo1-grp-t1` and `arquivo2-grp-t1`). Large generated benchmark inputs should not be committed to Git.

## Benchmark plan

**Input-size scaling:** compare the sequential and parallel versions with the following multipliers of `arquivo2-0`, using **4 threads** for parallel runs:

- 1×
- 1,000×
- 100,000×
- 200,000×

**Thread scaling:** run the parallel implementations with **1, 2, 4, and 8 threads**, using the 200,000× input.

For each configuration, report the **mean execution time** across repeated runs (e.g., 10) and its **standard deviation**. Compare execution times, speedups, and scalability, explaining any improvements or regressions. Check the occurrence count against the original sequential implementation.

## Technical requirements

- Language: **C** (other languages are not accepted for the implementation).
- Parallel framework: **OpenMP**, compatible with the version installed on the course's remote machine.
- Correctness: count matches without overlapping occurrences, matching the supplied reference output.

## Deliverables and dates

- **December 2, 2026, 10:00:** submit both parallel implementations, the optimized sequential version, and a report discussing optimization strategies and performance measurements.
- **December 3, 2026:** performance competition on the remote machine.

## Status

Project scaffold and experimental plan. Implementation details, build instructions, and benchmark results will be added as development progresses.

## Assignment

Source: *Enunciado do Projeto 2026*, PCS3868 — Sistemas de Computação de Alto Desempenho, USP.
