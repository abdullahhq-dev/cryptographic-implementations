# RSA in C

This project implements basic RSA operations in C using GMP, based on the RSA primitives described in RFC 8017. It includes key generation, raw RSA encryption and decryption, CRT-based decryption, and benchmarks comparing the performance of these operations. An alternative implementation uses GMP's `mpz_powm_sec` for constant-time-oriented modular exponentiation.

## Files

- `rsa.c` — RSA implementation using `mpz_powm`.
- `rsa_const.c` — alternative implementation using `mpz_powm_sec`.
- `rsa.h` — shared declarations and RSA parameters.
- `test.c` — tests normal RSA and CRT decryption by checking that each recovers the original message.
- `benchmark.c` — benchmarks RSA encryption, normal decryption, and CRT decryption.
- `BENCHMARK.md` — benchmark results, GMP function microbenchmarks, and performance analysis.

Use either `rsa.c` or `rsa_const.c` when building a program; do not compile both together because they define the same functions.

## Requirements

- GCC
- GMP development library

On Ubuntu, install GMP with:

```sh
sudo apt install libgmp-dev
```

## Correctness test

Build and run the test with the regular implementation:

```sh
gcc rsa.c test.c -lgmp -o rsa_test
./rsa_test
```

To build the test with `mpz_powm_sec`, use `rsa_const.c` in place of `rsa.c`.

## Benchmark

Build and run the benchmark, for example with GCC optimization level `-O3`:

```sh
gcc -O3 -march=native rsa.c benchmark.c -lgmp -o rsa_benchmark
./rsa_benchmark
```

Use `rsa_const.c` instead of `rsa.c` to benchmark the `mpz_powm_sec` version. Results and further analysis are available in [`BENCHMARK.md`](BENCHMARK.md). Timings depend on the machine, compiler, and GMP version.

## Scope and security

This is an educational implementation. The encryption function performs raw RSA modular exponentiation; RSAES-OAEP and other PKCS #1 encoding schemes. The key-generation code seeds GMP's pseudorandom generator with `time(NULL)`, which is not suitable for generating production cryptographic keys.