# ChaCha20 Benchmarks
**Processor:** Intel Core i3-6006U (6th Generation), 2.0 GHz, 2 cores, 3 MB cache  
**Compiler:** GCC  
**Message size:** 64 bytes  
**Iterations:** 1,000,000

## Results

| Optimization | Cycles/iteration | Cycles/byte | Time/iteration (ns) | Time/byte (ns) |
|---|---:|---:|---:|---:|
| `-O0` | 8519.84 | 133.1226 | 4277.19 | 66.83 |
| `-O2` | 1229.11 | 19.2048 | 617.05 | 9.64 |
| `-O3` | 719.10 | 11.2359 | 361.01 | 5.64 |

Results are from this machine and may vary with hardware, compiler version, and system load.
