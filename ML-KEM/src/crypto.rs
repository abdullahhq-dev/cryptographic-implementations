#![allow(dead_code)]
#![allow(non_snake_case)]

pub type B32 = [u8; 32];
pub type Polynomial = [u16; 256];
pub type NttPolynomial = [u16; 256];


use sha3::{Digest, Sha3_256, Sha3_512};

use shake::digest::{ExtendableOutput, Update, XofReader};
use shake::{Shake128, Shake256};

pub fn G(x: impl AsRef<[u8]>) -> (B32, B32) {
    let mut hash = Sha3_512::new();
    Digest::update(&mut hash, x);
    let out = hash.finalize();

    let mut a = B32::default();
    let mut b = B32::default();

    a.copy_from_slice(&out[..32]);
    b.copy_from_slice(&out[32..]);
    (a, b)
}

pub fn H(x: impl AsRef<[u8]>) -> B32 {
    let mut hash = Sha3_256::new();
    Digest::update(&mut hash, x);
    hash.finalize().into()
}

pub fn J(x: impl AsRef<[u8]>) -> B32 {
    let mut hash = Shake256::default();
    Update::update(&mut hash, x.as_ref());
    let mut out: B32 = [0; 32];
    let mut reader = hash.finalize_xof();
    reader.read(&mut out);
    out
}

pub fn PRF<const N: usize>(s: &B32, b: u8) -> [u8; N] {
    // N = 128(192) when EETA = 2(3)
    let mut hash = Shake256::default();
    Update::update(&mut hash, s);
    Update::update(&mut hash, &[b]);
    let mut out = [0u8; N];
    let mut reader = hash.finalize_xof();
    reader.read(&mut out);
    out
}
// The below function initialise and return a ctx mentioned in the standard with input of 34 bytes
pub fn XOF(rho: &B32, i: u8, j: u8) -> impl XofReader {
    let mut hash = Shake128::default();
    Update::update(&mut hash, rho);
    Update::update(&mut hash, &[i, j]);
    hash.finalize_xof()
}

pub fn bits_to_bytes(b: &[u8]) -> Vec<u8> {
    let mut out = vec![0u8; b.len() / 8];
    for i in 0..b.len() {
        out[i / 8] |= b[i] << (i % 8); // each bit in b is u8. so concatenating 8 of them together to form a bit.
    }
    out
}
pub fn bytes_to_bits(by: &[u8]) -> Vec<u8> {
    let mut out = vec![0u8; by.len() * 8];
    for i in 0..by.len() {
        for j in 0..8 {
            out[8 * i + j] = 1 & (by[i] >> j);
        }
    }
    out
}
pub fn byte_encode<const D: usize>(f: &Polynomial) -> Vec<u8> {
    let mut b = vec![0u8; 256 * D];

    for i in 0..256 {                     //
        let mut a = f[i];
        for j in 0..D {
            b[i * D + j] = (a % 2) as u8;       // each b[k]is either 0 or 1.
            a /= 2;
        }
    }
    bits_to_bytes(&b)
}

pub fn byte_encode_vec<const D: usize, const K:usize>(v: &[Polynomial;K]) -> Vec<u8> {
    let mut out = Vec::new();
    for i in 0..K {
        out.extend(byte_encode::<D>(&v[i]));
    }
    out
}

pub fn byte_decode<const D: usize>(by: &[u8]) -> Polynomial {
    let b = bytes_to_bits(by);
    let mut f: Polynomial = [0u16; 256];

    let m = if D < 12 { 1u16 << D } else { 3329 };
    for i in 0..256 {
        for j in 0..D {
            f[i] |= (b[i * D + j] as u16) << j;
        }
    }
    if D == 12 {
        for i in 0..256 {
            f[i] %= m;
        }
    }
    f
}

pub fn byte_decode_vec<const D: usize, const K:usize>(by: &[u8]) -> [Polynomial; K] {
    let mut out = [[0u16;256];K];
    let bytes_per_poly = 32 * D;    // 256 / 8 = 32
    for i in 0..K {
        let start = i * bytes_per_poly;     
        let end = start + bytes_per_poly;       // start end gives index of each element in the vector
        out[i] = byte_decode::<D>(&by[start..end]);
    }
    out
}

pub fn compress<const D: usize>(x: u16) -> u16 {
    let two_to_d = 1u32 << D;
    let y = (x as u32 * two_to_d + 1664) / 3329;
    (y % two_to_d) as u16
}

pub fn compress_poly<const D: usize>(f:&Polynomial) -> Polynomial {
    let mut out = [0u16;256];
    for i in 0..256 {
        out[i] = compress::<D>(f[i]);
    }
    out
}

pub fn compress_vec<const D: usize, const K: usize> (v: &[Polynomial;K]) -> [Polynomial;K] {
    let mut out =[[0u16;256];K];
    for i in 0..K {
        out[i] = compress_poly::<D>(&v[i]);
    }
    out
}

pub fn decompress<const D: usize>(y: u16) -> u16 {
    let two_to_d = 1u32 << D;
    let decompressed = (y as u32 * 3329u32 + (two_to_d >> 1)) / two_to_d;
    decompressed as u16
}

pub fn decompress_poly<const D: usize>(f: &Polynomial) -> Polynomial {
    let mut out = [0u16;256];
    for i in 0..256 {
        out [i] = decompress::<D>(f[i]);
    }
    out
}

pub fn decompress_vec<const D: usize, const K: usize> (v: &[Polynomial; K]) -> [Polynomial; K] {
    let mut out = [[0u16;256]; K];
    for i in 0..K {
        out[i] = decompress_poly::<D>(&v[i]);
    }
    out
}

pub fn sample_ntt(b: &B32, i: u8, j: u8) -> NttPolynomial {
    let mut f: NttPolynomial = [0u16; 256];
    let mut ctx = XOF(b, i, j);
    let mut C = [0u8; 3];
    let mut k: usize = 0;

    while k < 256 {
        ctx.read(&mut C);

        let d1:u16 = (C[0] as u16) + ((C[1] as u16) & 15u16) << 8; // full 8 bits of C[0]  and appending the least significant 4 bits of C[1]
        let d2:u16 = ((C[1] as u16) >> 4) + ((C[2] as u16) << 4);   // most significant 4 bits of C[1] and appending full 8 bits of C[2] 

        if d1 < 3329 {
            f[k] = d1;
            k += 1;
        }

        if d2 < 3329 && k < 256 {
            f[k] = d2;
            k += 1;
        }
    }
    f
}

pub fn SamplePolyCBD<const EETA: usize> (B: &[u8]) -> Polynomial {
    let mut f: Polynomial =[0u16;256];

    let b = bytes_to_bits(B);
    for i in 0..256 {
        let mut x = 0u16;
        let mut y = 0u16;
        for j in 0..EETA {
            x += b[2 * i * EETA + j] as u16;
            y += b[2 * i * EETA + EETA + j] as u16;
        }
        f[i] = (x + 3329 - y) % 3329 ;
    }
    f
}