# ML-KEM in Rust

An educational implementation of **ML-KEM (Kyber)** in Rust, based on
[FIPS 203](https://doi.org/10.6028/NIST.FIPS.203).

The goal of this project is to understand the ML-KEM specification and
practice implementing cryptographic algorithms in Rust.

## Status

Currently implementing **ML-KEM-512**.

Implemented so far:

- SHA3 / SHAKE based cryptographic primitives
- Byte encoding and decoding
- Polynomial compression and decompression
- CBD sampling
- NTT and inverse NTT
- Polynomial and vector operations
- ML-KEM PKE:
  - Key generation
  - Encryption
  - Decryption
- ML-KEM internal algorithms:
  - `KeyGen_internal`
  - `Encaps_internal`
  - `Decaps_internal`
- ACVP test-vector validation for selected cases

## Project Structure

```text
src/
├── crypto.rs            # Hashes, sampling, encoding, compression
├── ntt.rs               # NTT and polynomial multiplication
├── pke.rs               # ML-KEM PKE algorithms
├── ml_kem_internal.rs   # ML-KEM internal algorithms 
└── lib.rs               # Crate modules
