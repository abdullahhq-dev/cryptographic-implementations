#![allow(dead_code)]
#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(unused_assignments)]
#![allow(unused_imports)]
#![allow(unused_variables)]

use crate::crypto::{B32, NttPolynomial, PRF, Polynomial, compress_poly, compress_vec, decompress, decompress_poly, decompress_vec};
use crate::crypto::{G, SamplePolyCBD, byte_decode, byte_decode_vec, byte_encode, byte_encode_vec, sample_ntt};
use crate::ntt::{INTT, MultiplyNTTs, NTT, NTT_Vec, INTT_Vec, vec_dot_ntt};

pub type Matrix = [[NttPolynomial; 2]; 2];
pub type ek_pke = [u8; 800]; // B384k+32
pub type dk_pke = [u8; 768]; //B384k

pub fn poly_add(f: &Polynomial, g: &Polynomial) -> Polynomial {
    let mut h: Polynomial = [0u16; 256];
    for i in 0..256 {
        h[i] = (f[i] + g[i]) % 3329;
    }
    h
}

pub fn vec_add(a: &[Polynomial; 2], b: &[Polynomial; 2]) -> [Polynomial; 2] {
    let mut c: [Polynomial; 2] = [[0u16; 256]; 2];
    for i in 0..2 {
        c[i] = poly_add(&a[i], &b[i]);
    }
    c
}

pub fn poly_subtract(f: &Polynomial, g: &Polynomial) -> Polynomial {
    let mut out: Polynomial = [0u16; 256];
    for i in 0..256 {
        out[i] = (f[i]+ 3329 - g[i] ) % 3329;
    }
    out
}

pub fn matrix_vec_mul(A_hat: &Matrix, s: &[NttPolynomial; 2]) -> [NttPolynomial; 2] {
    let mut t: [NttPolynomial; 2] = [[0u16; 256]; 2];

    for i in 0..2 {
        for j in 0..2 {
            let product = MultiplyNTTs(&A_hat[i][j], &s[j]);
            t[i] = poly_add(&t[i], &product);
        }
    }
    t
}

pub fn matrix_transpose_vec_mul(A_hat: &Matrix, s: &[NttPolynomial; 2]) -> [NttPolynomial; 2] {
    let mut t: [NttPolynomial; 2] = [[0u16; 256]; 2];

    for i in 0..2 {
        for j in 0..2 {
            let product = MultiplyNTTs(&A_hat[j][i], &s[j]);
            t[i] = poly_add(&t[i], &product);
        }
    }
    t
}

pub fn KeyGen(seed: &B32) -> (ek_pke, dk_pke) {
    let mut input = [0u8; 33];
    input[..32].copy_from_slice(seed);
    input[32] = 2;

    let (rho, sigma) = G(input);

    let mut N = 0u8;
    let mut A_hat: Matrix = [[[0u16; 256]; 2]; 2];

    for i in 0..2 {
        for j in 0..2 {
            A_hat[i][j] = sample_ntt(&rho, j as u8, i as u8); // write as polynomial later
        }
    }

    let mut s: [NttPolynomial; 2] = [[0u16; 256]; 2]; // write as Polynomial later
    let mut e: [NttPolynomial; 2] = [[0u16; 256]; 2];

    for i in 0..2 {
        s[i] = SamplePolyCBD::<3>(&(PRF::<192>(&sigma, N)));
        N += 1;
    }

    for i in 0..2 {
        e[i] = SamplePolyCBD::<3>(&(PRF::<192>(&sigma, N)));
        N += 1;
    }

    let mut s_hat: [NttPolynomial; 2] = [[0u16; 256]; 2];
    let mut e_hat: [NttPolynomial; 2] = [[0u16; 256]; 2];

    for i in 0..2 {
        s_hat[i] = NTT(&s[i]);
        e_hat[i] = NTT(&e[i]);
    }

    // now s and e is in ntt form. now I need to generate as+e where I should use nttmult

    let t_hat: [NttPolynomial; 2] = vec_add(&matrix_vec_mul(&A_hat, &s_hat), &e_hat);
    let t_bytes = byte_encode_vec::<12, 2>(&t_hat);

    let mut encryption_key: ek_pke = [0u8; 800];

    encryption_key[..768].copy_from_slice(&t_bytes);
    encryption_key[768..].copy_from_slice(&rho);

    let s_bytes = byte_encode_vec::<12, 2>(&s_hat);
    let mut decryption_key: dk_pke = [0u8; 768];
    decryption_key.copy_from_slice(&s_bytes);

    (encryption_key, decryption_key)
}

pub fn Encrypt(encryption_key: &ek_pke, m: &B32, r: &B32) -> [u8; 768] {
    let mut N = 0u8;
    let t_hat = byte_decode_vec::<12, 2>(&encryption_key[..768]);

    let rho: [u8; 32] = encryption_key[768..800].try_into().unwrap();
    let mut A_hat: Matrix = [[[0u16; 256]; 2]; 2];
    for i in 0..2 {
        for j in 0..2 {
            A_hat[i][j] = sample_ntt(&rho, j as u8, i as u8); // write as polynomial later
        }
    }

    let mut y: [Polynomial; 2] = [[0u16; 256]; 2];
    for i in 0..2 {
        y[i] = SamplePolyCBD::<3>(&(PRF::<192>(r, N)));
        N += 1;
    }

    let mut e_1: [Polynomial; 2] = [[0u16; 256]; 2];
    for i in 0..2 {
        e_1[i] = SamplePolyCBD::<2>(&(PRF::<128>(r, N)));
        N += 1;
    }
    
    let e_2 = SamplePolyCBD::<2>(&(PRF::<128>(r, N )));

    let y_hat =NTT_Vec::<2>(&y);

    let u = vec_add(&INTT_Vec(&matrix_transpose_vec_mul(&A_hat, &y_hat)), &e_1);

    let mu = decompress_poly::<1>(&byte_decode::<1>(m));

    let y_t = INTT(&vec_dot_ntt(&t_hat, &y_hat));
    let v = poly_add(&y_t, &e_2);
    let v = poly_add(&v, &mu);

    let c1 = compress_vec::<10, 2>(&u);
    let c1 = byte_encode_vec::<10, 2>(&c1);

    let c2 = compress_poly::<4>(&v);
    let c2 = byte_encode::<4>(&c2);

    let mut c = [0u8;768];
    c[..640].copy_from_slice(&c1);
    c[640..].copy_from_slice(&c2);

    c
}

pub fn Decrypt(decryption_key: &dk_pke, c: &[u8;768]) -> B32 {
    let c1 = &c[..640];
    let c2 = &c[640..];

    let u = byte_decode_vec::<10, 2>(c1);
    let u = decompress_vec::<10, 2>(&u);
    let u_hat = NTT_Vec::<2>(&u);


    let v = byte_decode::<4>(c2);
    let v = decompress_poly::<4>(&v);

    let s_hat = byte_decode_vec::<12, 2>(decryption_key);

    let sT_u_hat = vec_dot_ntt(&s_hat, &u_hat);
    let sT_u = INTT(&sT_u_hat);

    let w = poly_subtract(&v, &sT_u);

    let m = compress_poly::<1>(&w);
    let m = byte_encode::<1>(&m);

    m.try_into().expect("message must be 32 bytes")
}
