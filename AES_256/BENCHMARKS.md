# AES-256 Benchmarks

**Processor:** Intel Core i3-6006U (6th Generation), 2.0 GHz, 2 cores, 3 MB cache  
**Compiler:** GCC  
**Iterations:** 100,000 per run

## Block Encryption

| Optimization | Cycles per block | Time per block (ns) | Cycles per byte |
|---|---:|---:|---:|
| `-O0` | 12,680.38 | 6,365.77 | 792.5234 |
| `-O1` | 1,837.04 | 922.24 | 114.8149 |
| `-O2` | 1,660.08 | 833.40 | 103.7552 |
| `-O3` | 1,257.39 | 631.24 | 78.5868 |

The ciphertext matched the expected AES-256 test vector:

```text
8ea2b7ca516745bfeafc49904b496089
```

## Key Expansion

| Optimization | Cycles per key expansion | Time per key expansion (ns) | Cycles per byte |
|---|---:|---:|---:|
| `-O0` | 4,291.52 | 2,154.44 | 17.8813 |
| `-O1` | 1,469.71 | 737.83 | 6.1238 |
| `-O2` | 1,066.06 | 535.19 | 4.4419 |
| `-O3` | 778.52 | 423.42 | 3.2438 |

Results are from this machine and may vary with hardware, compiler version, and system load.
