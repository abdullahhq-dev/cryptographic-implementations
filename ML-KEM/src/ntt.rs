#![allow(dead_code)]
#![allow(non_snake_case)]

use crate::pke::{poly_add};
pub type B32 = [u8; 32];
pub type Polynomial = [u16; 256];
pub type NttPolynomial = [u16; 256];

const ZETAS: [u16; 128] = [
    1, 1729, 2580, 3289, 2642, 630, 1897, 848, 1062, 1919, 193, 797, 2786, 3260, 569, 1746, 296,
    2447, 1339, 1476, 3046, 56, 2240, 1333, 1426, 2094, 535, 2882, 2393, 2879, 1974, 821, 289, 331,
    3253, 1756, 1197, 2304, 2277, 2055, 650, 1977, 2513, 632, 2865, 33, 1320, 1915, 2319, 1435,
    807, 452, 1438, 2868, 1534, 2402, 2647, 2617, 1481, 648, 2474, 3110, 1227, 910, 17, 2761, 583,
    2649, 1637, 723, 2288, 1100, 1409, 2662, 3281, 233, 756, 2156, 3015, 3050, 1703, 1651, 2789,
    1789, 1847, 952, 1461, 2687, 939, 2308, 2437, 2388, 733, 2337, 268, 641, 1584, 2298, 2037,
    3220, 375, 2549, 2090, 1645, 1063, 319, 2773, 757, 2099, 561, 2466, 2594, 2804, 1092, 403,
    1026, 1143, 2150, 2775, 886, 1722, 1212, 1874, 1029, 2110, 2935, 885, 2154,
];

const ZETAS_INV: [u16; 128] = [
    17, 3312, 2761, 568, 583, 2746, 2649, 680, 1637, 1692, 723, 2606, 2288, 1041, 1100, 2229, 1409,
    1920, 2662, 667, 3281, 48, 233, 3096, 756, 2573, 2156, 1173, 3015, 314, 3050, 279, 1703, 1626,
    1651, 1678, 2789, 540, 1789, 1540, 1847, 1482, 952, 2377, 1461, 1868, 2687, 642, 939, 2390,
    2308, 1021, 2437, 892, 2388, 941, 733, 2596, 2337, 992, 268, 3061, 641, 2688, 1584, 1745, 2298,
    1031, 2037, 1292, 3220, 109, 375, 2954, 2549, 780, 2090, 1239, 1645, 1684, 1063, 2266, 319,
    3010, 2773, 556, 757, 2572, 2099, 1230, 561, 2768, 2466, 863, 2594, 735, 2804, 525, 1092, 2237,
    403, 2926, 1026, 2303, 1143, 2186, 2150, 1179, 2775, 554, 886, 2443, 1722, 1607, 1212, 2117,
    1874, 1455, 1029, 2300, 2110, 1219, 2935, 394, 885, 2444, 2154, 1175,
];

pub fn NTT(f: &Polynomial) -> NttPolynomial {
    let mut fntt: NttPolynomial = *f;

    let mut zeta: u16;
    let mut t: u16;

    let mut i = 1;
    let mut len = 128;
    while len >= 2 {
        let mut start = 0;
        while start < 256 {
            zeta = ZETAS[i];
            i += 1;
            for j in start..start + len {
                t = ((zeta as u32 * fntt[j + len] as u32) % 3329) as u16;
                fntt[j + len] = (fntt[j] + 3329 - t) % 3329;
                fntt[j] = (fntt[j] + t) % 3329;
            }
            start += 2 * len;
        }
        len /= 2;
    }
    fntt
}

pub fn INTT(fntt: &NttPolynomial) -> Polynomial {
    let mut f = *fntt;
    let mut i = 127;
    let mut len = 2;
    let mut zeta: u16;
    let mut t: u16;
    while len <= 128 {
        let mut start = 0;
        while start < 256 {
            zeta = ZETAS[i];
            i -= 1;
            for j in start..start + len {
                t = f[j];
                f[j] = (t + f[j + len]) % 3329;
                f[j + len] =
                    ((zeta as u32 * ((f[j + len] + 3329 - t) % 3329) as u32) % 3329) as u16;
            }
            start += 2 * len;
        }
        len *= 2;
    }
    for x in f.iter_mut() {
        *x = ((*x as u32 * 3303u32) % 3329u32) as u16; // 3303 = inv 128 (mod q).
    }
    f
}

pub fn BaseCaseMultiply(a0: u16, a1: u16, b0: u16, b1: u16, gamma: u16) -> (u16, u16) {
    let t = (a1 as u32 * b1 as u32) % 3329;
    let c0 = ((a0 as u32 * b0 as u32 + t * gamma as u32) % 3329) as u16;
    let c1 = ((a0 as u32 * b1 as u32 + a1 as u32 * b0 as u32) % 3329) as u16;
    (c0, c1)
}

pub fn MultiplyNTTs(f: &NttPolynomial, g: &NttPolynomial) -> NttPolynomial {
    let mut h: NttPolynomial = [0u16; 256];
    for i in 0..128 {
        (h[2 * i], h[2 * i + 1]) =
            BaseCaseMultiply(f[2 * i], f[2 * i + 1], g[2 * i], g[2 * i + 1], ZETAS_INV[i]);
    }
    h
}

pub fn vec_dot_ntt<const K: usize>(t_hat: &[NttPolynomial;K], y_hat: &[NttPolynomial;K]) -> NttPolynomial {
    let mut out = [0u16;256];
    for i in 0..K {
        let product = MultiplyNTTs(&t_hat[i], &y_hat[i]);
        out = poly_add(&out, &product);
    }
    out
}

pub fn NTT_Vec<const K: usize>(y: &[Polynomial;K]) -> [NttPolynomial;K] {
    let mut out =[[0u16;256];K];
    for i in 0..K {
        out[i] = NTT(&y[i]);
    }
    out
} 

pub fn INTT_Vec<const K: usize>(y: &[NttPolynomial;K]) -> [Polynomial;K] {
    let mut out = [[0u16;256];K];
    for i in 0..K {
        out[i] = INTT(&y[i]);
    }
    out
}
