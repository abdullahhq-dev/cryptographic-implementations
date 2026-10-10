
# ML-KEM in Rust

An ongoing, from-scratch implementation of ML-KEM (Kyber) in Rust, strictly following
[NIST FIPS 203](https://doi.org/10.6028/NIST.FIPS.203).

## Status

Currently implementing **ML-KEM-512**.

Implemented so far:

- SHA3 / SHAKE based cryptographic primitives
- Byte encoding and decoding
- Polynomial compression and decompression
- Centered Binomial Distribution (CBD) sampling
- NTT and inverse NTT
- Polynomial and vector operations
- ML-KEM PKE:
  - Key generation
  - Encryption
  - Decryption
- ML-KEM internal algorithms:
  - KeyGen_internal
  - Encaps_internal
  - Decaps_internal
- Public ML-KEM API:
  - Key generation
  - Encapsulation
  - Decapsulation
- Selected ACVP test-vector validation

## Testing

Selected ACVP test vectors from the NIST ML-KEM validation suite are included
as unit tests in `ml_kem_internal.rs`.

The tests currently cover:

- ML-KEM key generation
- ML-KEM encapsulation
- ML-KEM decapsulation
- Implicit rejection for modified ciphertexts

Run all tests with:

```bash
cargo test
```

The ACVP vectors are currently included as selected test cases for manual
validation. More comprehensive automated test-vector coverage will be added
as the project develops.

## Project Structure

```text
src/
├── crypto.rs            # Hashes, sampling, encoding and compression
├── ntt.rs               # NTT, inverse NTT and polynomial multiplication
├── pke.rs               # ML-KEM PKE algorithms
├── ml_kem_internal.rs   # ML-KEM internal algorithms and ACVP tests
├── ml_kem.rs            # Public ML-KEM API
└── lib.rs               # Crate modules
```

## Dependencies

The implementation currently uses the following external crates:

- [`sha3`](https://crates.io/crates/sha3) — SHA3-256 and SHA3-512
- [`shake`](https://crates.io/crates/shake) — SHAKE128 and SHAKE256
- [`getrandom`](https://crates.io/crates/getrandom) — operating-system
  randomness for the public API
- [`subtle`](https://crates.io/crates/subtle) — constant-time comparison and
  conditional selection
- [`hex`](https://crates.io/crates/hex) — hexadecimal encoding and decoding
  for test vectors

## Pending Work

### Correctness and API

- [ ] Improve error handling throughout the codebase
- [ ] Ensure constant-time implementation throughout the codebase
- [ ] Expand ACVP test-vector coverage
- [ ] Add more comprehensive unit and known-answer tests

### Memory and Data Representation

- [ ] Remove unnecessary heap allocations
- [ ] Replace polynomial type aliases with dedicated `Polynomial` and
      `NttPolynomial` structs
- [ ] Add secure memory zeroization for sensitive intermediate values

### Performance

- [ ] Implement Barrett and Montgomery reduction where appropriate
- [ ] Benchmark the implementation
- [ ] SIMD-based hardware acceleration

### Extensions

- [ ] Add support for ML-KEM-768
- [ ] Add support for ML-KEM-1024



