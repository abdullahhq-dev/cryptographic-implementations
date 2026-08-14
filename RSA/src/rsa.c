#include <stdlib.h>

#include "rsa.h"

// The implementation uses time(NULL) as a seed for 
// educational purposes and is not suitable for production-level RSA key generation.

void rsa_rand_init(gmp_randstate_t state) {
  gmp_randinit_default(state);
  gmp_randseed_ui(state, time(NULL));
}

void rsa_generate_prime( mpz_t p, gmp_randstate_t state, const mp_bitcnt_t bits) {
  mpz_urandomb(p, state, bits);
  mpz_setbit(p, bits - 1);
  mpz_nextprime(p, p);
}

void rsa_keygen(gmp_randstate_t state, mpz_t n, mpz_t e, mpz_t d, mpz_t p, mpz_t q, mpz_t dP, mpz_t dQ, mpz_t q_inv) {
  mpz_t p_minus_1, q_minus_1, lambda_n, gcd;
  mpz_inits(lambda_n, gcd, p_minus_1, q_minus_1, NULL);
  mpz_set_ui(e, PUBLIC_EXPONENT);
  
  do {
    do {
      rsa_generate_prime(p, state, PRIME_SIZE);
      rsa_generate_prime(q, state, PRIME_SIZE);
    } while (mpz_cmp(p, q) == 0); // Regenerating p and q if they are not distinct
    
    mpz_mul(n, p, q);
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_sub_ui(q_minus_1, q, 1);
    mpz_lcm(lambda_n, p_minus_1, q_minus_1);
    mpz_gcd(gcd, lambda_n, e);
    
  } while (mpz_cmp_ui(gcd, 1) != 0);  // Regenerating p and q if e and lambda_n is not coprime
  
  if (mpz_invert(d, e, lambda_n) == 0)
    exit(EXIT_FAILURE);
    
  mpz_mod(dP, d, p_minus_1);
  mpz_mod(dQ, d, q_minus_1);
  if (mpz_invert(q_inv, q, p) == 0)
    exit(EXIT_FAILURE);
  
  
  mpz_clears(lambda_n, gcd, p_minus_1, q_minus_1, NULL);
}

void rsa_encrypt(mpz_t c, const mpz_t m, const mpz_t e, const mpz_t n) {
  if (mpz_cmp(m, n) >= 0) // error
    exit(EXIT_FAILURE);
  mpz_powm(c, m, e, n); 
}

void rsa_decrypt_crt(mpz_t m, const mpz_t c,  const mpz_t n, const mpz_t p, const mpz_t q, const mpz_t dP, const mpz_t dQ, const mpz_t q_inv) {
  mpz_t m_1, m_2, h;
  mpz_inits(m_1, m_2, h, NULL);
  mpz_powm(m_1, c, dP, p);
  mpz_powm(m_2, c, dQ, q);
  
  mpz_sub(h, m_1, m_2);
  mpz_mul(h, h, q_inv);
  mpz_mod(h, h, p);
  
  mpz_mul(m, q, h);
  mpz_add(m, m, m_2);
  
  mpz_clears(m_1, m_2, h, NULL);
}

void rsa_decrypt(mpz_t m, const mpz_t c, const mpz_t n, const mpz_t d) {
  mpz_powm(m, c, d, n);
}


