#![allow(dead_code)]
#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(unused_assignments)]
#![allow(unused_imports)]
#![allow(unused_variables)]

use crate::crypto::{B32, G, H, J};
use crate::pke::{Decrypt, Encrypt, KeyGen};

pub type ek_internal = [u8; 800];
pub type dk_internal = [u8; 1632];
pub type KEY = B32;
pub type CIPHER = [u8; 768];

pub fn KeyGen_internal(d: &B32, z: &B32) -> (ek_internal, dk_internal) {
    let (ek, dk) = KeyGen(d);
    let encaps_key = ek;

    let hash_ek = H(&encaps_key);

    let mut decaps_key = [0u8; 1632];
    decaps_key[..768].copy_from_slice(&dk);
    decaps_key[768..1568].copy_from_slice(&encaps_key);
    decaps_key[1568..1600].copy_from_slice(&hash_ek);
    decaps_key[1600..].copy_from_slice(z);
    (encaps_key, decaps_key)
}

// below m is randomness. IDK why it is named m in the specification
pub fn Encaps_internal(encaps_key: &ek_internal, m: &B32) -> (KEY, CIPHER) {
    let mut m_H_ek = [0u8; 64];
    m_H_ek[..32].copy_from_slice(m);
    m_H_ek[32..].copy_from_slice(&H(encaps_key));

    let (K, r) = G(m_H_ek);
    let c = Encrypt(encaps_key, m, &r);

    (K, c)
}

pub fn Decaps_internal(decaps_key: &dk_internal, c: &CIPHER) -> KEY {
    let dk: [u8; 768] = decaps_key[0..768].try_into().unwrap();
    let ek: [u8; 800] = decaps_key[768..1568].try_into().unwrap();
    let h: B32 = decaps_key[1568..1600].try_into().unwrap();
    let z: B32 = decaps_key[1600..].try_into().unwrap();

    let m_prime = Decrypt(&dk, c);

    let mut m_prime_h = [0u8; 64];
    m_prime_h[..32].copy_from_slice(&m_prime);
    m_prime_h[32..].copy_from_slice(&h);
    let (mut K_prime, r_prime) = G(&m_prime_h);

    let mut z_c = [0u8; 64];
    z_c[..32].copy_from_slice(&z);
    z_c[32..].copy_from_slice(c);
    let K_bar = J(&z_c);

    let c_prime = Encrypt(&ek, &m_prime, &r_prime);

    if *c != c_prime {
        K_prime = K_bar;
    }

    K_prime
}
