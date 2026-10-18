# ML-KEM-512 Benchmarks

This document records the performance comparison between this implementation
and the RustCrypto [`ml-kem`](https://github.com/RustCrypto/KEMs/tree/master/ml-kem)
reference implementation.

## Environment 

- **Laptop:** HP AY514TX
- **CPU:** Intel Core i3-6006U @ 2.00 GHz (2 cores, 4 threads)
- **Operating system:** Ubuntu 26.04.1 LTS, x86-64
- **Rust compiler:** `rustc 1.97.1` 
- **Benchmark harness:** Criterion.rs
- **Build profile:** Cargo's optimized `bench` profile

## Results

| Operation | This implementation | RustCrypto `ml-kem` | This / RustCrypto | RustCrypto lower latency by |
|---|---:|---:|---:|---:|
| Key generation | 84.527 µs | 46.407 µs | 1.82× | 45.1% |
| Encapsulation | 89.892 µs | 45.197 µs | 1.99× | 49.7% |
| Decapsulation | 126.54 µs | 59.005 µs | 2.14× | 53.4% |

### Criterion timing intervals

| Operation | This implementation | RustCrypto `ml-kem` |
|---|---:|---:|
| Key generation | 84.487–84.573 µs | 46.344–46.497 µs |
| Encapsulation | 89.865–89.924 µs | 45.173–45.223 µs |
| Decapsulation | 126.42–126.70 µs | 58.982–59.033 µs |

##

 Fixed byte arrays are used for deterministic key generation and
  encapsulation. A valid key pair and ciphertext are prepared before the timed
  sections; decapsulation is benchmarked using that fixed valid ciphertext.

Run both implementations' benchmarks with:

```bash
cargo bench --bench ml_kem
```


