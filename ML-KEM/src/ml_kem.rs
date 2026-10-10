use getrandom;
use crate::crypto::{byte_decode_vec, byte_encode_vec, H}; 
use crate::ml_kem_internal::{CIPHER, KEY, dk_internal, ek_internal, Decaps_internal, Encaps_internal, KeyGen_internal};


pub fn key_gen() -> Result<(ek_internal, dk_internal), getrandom::Error> {

    let mut d = [0u8;32];
    let mut z = [0u8;32];

    getrandom::fill(&mut d)?;
    getrandom::fill(&mut z)?;

    Ok(KeyGen_internal(&d, &z))
}

#[derive(Debug)]
pub enum EncapsError {
    InvalidEncapsulationKey,
    RandomnessFailure(getrandom::Error)
}

pub fn encaps(ek: &ek_internal) -> Result<(KEY, CIPHER), EncapsError> {
    let ek_bytes = &ek[..768];
    let test = byte_encode_vec::<12, 2>(&byte_decode_vec::<12, 2>(ek_bytes));
    if test != ek_bytes {
        return Err(EncapsError::InvalidEncapsulationKey);
    }

    let mut m = [0u8;32];
    getrandom::fill(&mut m).map_err(EncapsError::RandomnessFailure)?;

    Ok(Encaps_internal(ek, &m))
}

pub enum DecapsError {
    HashCheckFailed
}

pub fn decaps(dk: &dk_internal, c: &CIPHER) -> Result<KEY, DecapsError> {
    let embedded_ek = &dk[768..1568];
    let expected_hash = &dk[1568..1600];

    let computed_hash = H(embedded_ek);
    if computed_hash != expected_hash {
        return Err(DecapsError::HashCheckFailed);
    }
    Ok(Decaps_internal(dk, c))
}
