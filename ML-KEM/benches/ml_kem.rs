use criterion::{Criterion, criterion_group, criterion_main};
use std::hint::black_box;

// imports for the user built ml-kem crate
use ml_kem::ml_kem_internal::{Decaps_internal, Encaps_internal, KeyGen_internal};

// imports for rust_crypto ml-kem crate
use rustcrypto_ml_kem::{
    B32,
    FromSeed,
    MlKem512,
    Seed,
    kem::Decapsulate
};

const D: [u8; 32] = [0x11; 32];
const Z: [u8; 32] = [0x11; 32];
const M: [u8; 32] = [0x11; 32];


// benchmark function for user built ml-kem crate
fn bench_mlkem(c: &mut Criterion) {
    let (ek, dk) = KeyGen_internal(&D, &Z);
    let (key, cipher) = Encaps_internal(&ek, &M);
    let key_prime = Decaps_internal(&dk, &cipher);
    assert_eq!(key, key_prime);

    // Benchmark Key_Gen function

    c.bench_function("ML-KEM 512/key_gen", |b| {
        b.iter(||{
         black_box(KeyGen_internal(black_box(&D), black_box(&Z)))
        })
    });

    //  Benchmark encapsulation

    c.bench_function("ML-KEM 512/encapsulation", |b| {
        b.iter(|| {
            black_box(Encaps_internal(black_box(&ek),black_box(&M) ))
        } )
    });

    //  Benchmark decapsulation

    c.bench_function("ML-KEM 512/decapsulation", |b| {
        b.iter(|| {
            black_box(Decaps_internal(black_box(&dk), black_box(&cipher)))
        })
    });

}

// benchmark function for RustCrypto ml-kem crate

fn bench_rustcrypto(c: &mut Criterion) {
    // Combine the two 32-byte key-generation seeds.
    let mut seed_bytes = [0u8; 64];
    seed_bytes[..32].copy_from_slice(&D);
    seed_bytes[32..].copy_from_slice(&Z);

    let seed = Seed::from(seed_bytes);
    let m = B32::from(M);

    // Prepare reference keys and ciphertext outside timed sections.
    let (dk, ek) = MlKem512::from_seed(&seed);
    let (ct, ss_enc) = ek.encapsulate_deterministic(&m);

    // Verify correctness before benchmarking.
    assert_eq!(ss_enc, dk.decapsulate(&ct));

    // Key generation
    c.bench_function("RustCrypto ML-KEM-512/key_gen", |b| {
        b.iter(|| {
            black_box(
                MlKem512::from_seed(black_box(&seed))
            )
        })
    });

    // Encapsulation
    c.bench_function("RustCrypto ML-KEM-512/encapsulation", |b| {
        b.iter(|| {
            black_box(
                ek.encapsulate_deterministic(black_box(&m))
            )
        })
    });

    // Decapsulation
    c.bench_function("RustCrypto ML-KEM-512/decapsulation", |b| {
        b.iter(|| {
            black_box(dk.decapsulate(black_box(&ct)))
        })
    });
}


criterion_group!(benches, bench_mlkem, bench_rustcrypto);
criterion_main!(benches);
