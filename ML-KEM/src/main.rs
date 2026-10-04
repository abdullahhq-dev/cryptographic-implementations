#[allow(dead_code)]
mod crypto;
mod ntt;
mod pke;
mod ml_kem_internal;

// use crypto::{H,J};
use crate::crypto::{Polynomial, byte_decode, byte_encode};
fn main() {
    // let a:[u8; 32] = [0; 32];
    // let hash = H(a);
    // for byte in hash {
    //     print!("{:02x}", byte);
    // }
    // println!();

    // let v = vec![1, 2, 3];
    // J(&v);
    // println!("{:?}", v);
    let f1: Polynomial = [3228u16; 256];
    let f2: Polynomial = [0u16; 256];

    let bytes = byte_encode::<12>(&f1);
    let recovered = byte_decode::<12>(&bytes);

    assert_eq!(f1, recovered);

    let bytes = byte_encode::<4>(&f2);
    let recovered = byte_decode::<4>(&bytes);
    assert_eq!(f2, recovered);

    let mut f = [0u16; 256];
    f[0] = 1;
    f[1] = 2;
    f[2] = 3328;
    f[3] = 1234;

    let bytes = byte_encode::<12>(&f);
    assert_eq!(f, byte_decode::<12>(&bytes));
}
