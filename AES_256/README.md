# AES-256 in C

This project implements AES-256 block encryption and key expansion in C, following the AES specification in FIPS 197. It includes a correctness test using a standard test vector and benchmarks comparing performance across different GCC optimization levels. Block decryption and authenticated encryption modes remain to be implemented.

## Files

- `aes_256.c`, `aes_256.h` — AES-256 implementation and interface.
- `aes_256_correctness.c` — correctness test using a known-answer test vector from FIPS 197.
- `benchmark.c` — block encryption benchmark.
- `benchmark_keyexp.c` — key expansion benchmark.
- `BENCHMARKS.md` — recorded benchmark results.

## Run the correctness test

```bash
gcc  aes_256.c aes_256_correctness.c -o aes_test
./aes_test
```

Expected output:

```text
PASS: AES-256 known-answer test
```

## Benchmarks

Benchmark results and the processor used are documented in [BENCHMARKS.md](BENCHMARKS.md). The benchmark programs use x86 timestamp-counter instructions and are intended for x86 systems.


