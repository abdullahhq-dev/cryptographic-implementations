# ChaCha20 in C

This project implements the ChaCha20 stream cipher in C, following RFC 8439. It includes a correctness test using the RFC test vector and benchmarks comparing performance across different GCC optimization levels. The current implementation provides encryption without authentication; adding Poly1305 to support authenticated encryption (ChaCha20-Poly1305) remains future work.

## Files

- `chacha.c` — ChaCha20 implementation.
- `chacha.h` — type definitions and function declarations.
- `test_correctness.c` — checks the output against the RFC 8439 test vector.
- `benchmark.c` — measures encryption performance.
- `benchmark_results.md` — benchmark results.

## Correctness test

Compile and run:

```sh
gcc  chacha.c test_correctness.c -o test_correctness
./test_correctness
```

The test should print `PASS` when the generated ciphertext matches the expected ciphertext.

## Benchmark

Compile and run the benchmark, for example with `-O3`:

```sh
gcc -O3 chacha.c benchmark.c -o benchmark
./benchmark
```

Results for the tested optimization levels are recorded in [`BENCHMARKS.md`](BENCHMARKS.md). Benchmark timings depend on the hardware and compiler environment.
