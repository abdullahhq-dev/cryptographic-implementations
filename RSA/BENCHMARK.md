# RSA Benchmarks and GMP Analysis

## Benchmark Setup

- **Processor:** Intel Core i3-6006U (6th Generation), 2.0 GHz, 2 cores, 3 MB cache  
- **Library:** GMP
- **Compiler:** GCC with `-O3 -march=native`
- **RSA modulus:** approximately 2048 bits
- **Message size:** approximately 2047 bits
- **Iterations:** 10,000 per benchmark

The measurements below are from the original benchmark runs and are specific to the test machine and software environment.

## RSA Operations: `mpz_powm`

Compilation used `gcc -O3 -march=native`.

| Operation | Total cycles | Cycles per operation | Total time | Average time per operation |
|---|---:|---:|---:|---:|
| RSA encryption | 855.71 M | 85.57 k | 429.58 ms | 42.96 us |
| RSA-CRT decryption | 27,553.91 M | 2.755 M | 13.833 s | 1.383 ms |
| RSA decryption | 98,478.08 M | 9.848 M | 49.438 s | 4.944 ms |

For a 2048-bit RSA modulus, CRT reduced the measured decryption cost from 9.848 Mcycles (4.944 ms) to 2.755 Mcycles (1.383 ms), a speedup of approximately **3.57x**, or a **72% reduction** in measured cost.

## RSA Operations: `mpz_powm_sec`

In this run, `mpz_powm` was replaced by GMP's `mpz_powm_sec` in the RSA operations.

| Operation | Total cycles | Cycles per operation | Total time | Average time per operation |
|---|---:|---:|---:|---:|
| RSA encryption | 4,086.50 M | 408.65 k | 2,051.50 ms | 205.15 us |
| RSA-CRT decryption | 31,473.25 M | 3.147 M | 15.800 s | 1.580 ms |
| RSA decryption | 112,138.52 M | 11.214 M | 56.296 s | 5.630 ms |

Compared with the `mpz_powm` run:

- Encryption was approximately **4.8x slower**.
- CRT decryption was approximately **1.14x slower**.
- Ordinary decryption was approximately **1.14x slower**.

The public exponent is `e = 65537 = 2^16 + 1`, so ordinary modular exponentiation can exploit the small exponent. The `mpz_powm_sec` implementation is designed to reduce dependence on exponent values in its execution time and cache-access pattern for same-size arguments and identical machine state. These results illustrate a performance trade-off for the modular-exponentiation operation; they do not establish that the entire RSA implementation is constant-time.

## GMP Function Microbenchmarks

Individual GMP functions were microbenchmarked using approximately 2048-bit inputs, except where a different input size or exponent is specified.

| Function / inputs | Measured cycles per operation |
|---|---:|
| `mpz_mul` | 1,980.30 |
| `mpz_mod` | 92.64 |
| `mpz_invert` | 36,392.36 |
| `mpz_lcm` | 33,102.21 |
| `mpz_gcd` | 31,096.37 |
| `mpz_nextprime` | 224,004,088.50 |
| `mpz_powm` — random 2048-bit base, exponent, and modulus | 9,965,218.01 |
| `mpz_powm` — random 1024-bit base, exponent, and modulus | 1,365,702.87 |
| `mpz_powm` — exponent `e = 65537`, random 2048-bit base and modulus | 86,546.09 |
| `mpz_powm_sec` — random 2048-bit base, exponent, and modulus | 11,245,361.16 |
| `mpz_powm_sec` — random 1024-bit base, exponent, and modulus | 1,578,698.97 |
| `mpz_powm_sec` — exponent `e = 65537`, random 2048-bit base and modulus | 427,882.20 |

### Interpreting the microbenchmarks

#### Effect of operand size

For random 2048-bit base, modulus, and exponent, `mpz_powm` took approximately **9.965 M cycles per operation**. For random 1024-bit base, modulus, and exponent, it took approximately **1.366 M cycles per operation**.

The ratio is:

`9.965 / 1.366 ≈ 7.29`

Thus, reducing operand size from 2048 bits to 1024 bits reduced the measured modular-exponentiation cost by approximately **7.29x**. The 1024-bit operation cost about **13.7%** of the 2048-bit operation. The observed scaling depends on GMP's algorithms and operand sizes; it is not determined solely by the number of bits.

#### Relation to RSA decryption and CRT

Normal RSA decryption computes approximately:

`c^d mod n`

where `n` and `d` are approximately 2048 bits. The measured 2048-bit modular exponentiation cost was approximately **9.965 M cycles**, compared with **9.848 M cycles** for the ordinary RSA decryption benchmark. These values are close, indicating that the modular exponentiation dominates the measured cost.

CRT decryption computes two smaller modular exponentiations:

`m1 = c^dP mod p`

`m2 = c^dQ mod q`

where `p`, `q`, `dP`, and `dQ` are approximately 1024 bits. The microbenchmark estimate is:

`2 x 1.366 M = 2.732 M cycles`

The measured CRT decryption cost was approximately **2.755 M cycles**. The difference is **0.023 M cycles**, or approximately **0.84%**. This close agreement suggests that the two modular exponentiations account for almost all of the measured CRT decryption cost; the remaining cost includes CRT recombination and other implementation or benchmark overhead.

The observed decryption speedup was:

`9.848 / 2.755 ≈ 3.57x`

The corresponding reduction in measured cost was:

`1 - (2.755 / 9.848) ≈ 72.0%`

#### Why the CRT speedup is not exactly 4x

Halving operand size might suggest a 4x improvement under a simple quadratic-cost assumption. However, the measured modular-exponentiation scaling was approximately **7.29x** between 2048-bit and 1024-bit random operands. `mpz_powm` is a complete modular exponentiation operation; its cost depends on modular multiplication, modular reduction, exponentiation, operand sizes, and the algorithms GMP selects. Therefore, measured scaling is more informative than assuming a fixed quadratic relationship.

#### Effect of the public exponent `e = 65537`

The public exponent is:

`e = 65537 = 2^16 + 1`

For random 2048-bit base, modulus, and exponent, `mpz_powm` took approximately **9.965 M cycles**. With `e = 65537` and a random 2048-bit base and modulus, it took approximately **86.55 k cycles**.

The ratio is:

`9.965 M / 86.55 k ≈ 115x`

Thus, modular exponentiation with the small RSA public exponent was approximately **115x cheaper** in this microbenchmark. The RSA encryption benchmark measured **85.57 k cycles**, close to the **86.55 k-cycle** microbenchmark. This explains much of the difference between encryption and ordinary decryption: encryption uses the small public exponent, while decryption uses a private exponent of approximately 2048 bits.

#### `mpz_powm` versus `mpz_powm_sec`

For random operands, the measurements were:

| Inputs | `mpz_powm` | `mpz_powm_sec` | Approximate overhead |
|---|---:|---:|---:|
| 2048-bit base, exponent, and modulus | 9.965 M cycles | 11.245 M cycles | 1.13x (about 13%) |
| 1024-bit base, exponent, and modulus | 1.366 M cycles | 1.579 M cycles | 1.16x (about 16%) |
| Exponent `e = 65537`, 2048-bit base and modulus | 86.55 k cycles | 427.88 k cycles | 4.94x |

The relative performance cost was much larger for the small public exponent. The measurements show the following performance trade-off in these tests:

- `mpz_powm`: faster in these measurements; variable-time.
- `mpz_powm_sec`: slower in these measurements; designed for more consistent execution time and cache-access patterns under its documented conditions.

## Comparison with GMPbench

The RSA implementation was also compared with the `gmpbench` package available from the GMP website.

Command used:

```bash
gcc rsa.c -lgmp -O3
./a.out 2048
```

The recorded output included:

```text
Generating p, q, d...done; pq is 2048 bits
Precomputing CRT constants
Generating random messages
Calibrating CPU speed...done
Signing random messages 7272 times...done!
RESULT: 724 operations per second
```

GMPbench reported **724 operations per second**, corresponding to approximately **1.38 ms per operation**. The RSA-CRT implementation measured **1.383 ms per operation** in the benchmark above.

The two implementations use similar CRT structures: two modular exponentiations using the CRT exponents `dP` and `dQ`, followed by CRT recombination. The recorded analysis notes that GMPbench computes `p - 1 mod q` and reconstructs using `p`, while this implementation computes `q - 1 mod p` and reconstructs using `q`; these are equivalent CRT formulations. The benchmark methodologies are not identical, so the close timings are a useful sanity check rather than a strict apples-to-apples comparison.

## Overall Summary

The measurements are consistent with the following observations:

- **RSA encryption** is relatively fast because it uses the small public exponent `65537`; its measured cost was about **85.6 k cycles** with `mpz_powm`.
- **Ordinary RSA decryption** was dominated by a 2048-bit modular exponentiation and measured about **9.85 M cycles**.
- **CRT decryption** uses two approximately 1024-bit modular exponentiations and measured about **2.76 M cycles**, around **3.57x faster** than ordinary decryption in this run.
- The individual GMP microbenchmarks closely predicted the measured costs of encryption, ordinary decryption, and CRT decryption.
- `mpz_nextprime` was particularly expensive in the recorded microbenchmark, at approximately **224 million cycles per operation**.
- Replacing `mpz_powm` with `mpz_powm_sec` increased measured costs, especially for exponentiation with the small public exponent.

A possible next step is to examine GMP's implementation of `mpz_powm` to better understand how its modular exponentiation, multiplication, and reduction algorithms behave for different operand sizes.
