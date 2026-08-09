#include <stdlib.h>

#include "rsa.h"

// The implementation uses time(NULL) as a seed for 
// educational purposes and is not suitable for production RSA key generation.

void rsa_rand_init(gmp_randstate_t state) {
  gmp_randinit_default(state);
  gmp_randseed_ui(state, time(NULL));
}

void rsa_generate_prime( mpz_t p, gmp_randstate_t state, const mp_bitcnt_t bits) {
  mpz_urandomb(p, state, bits);
  mpz_setbit(p, bits - 1);
  mpz_nextprime(p, p);
}

void rsa_keygen( mpz_t n, mpz_t e, mpz_t d, gmp_randstate_t state) {
  mpz_t p, q, p_minus_1, q_minus_1, lambda_n, gcd;
  mpz_inits(p,q,lambda_n,gcd, p_minus_1, q_minus_1, NULL);
  mpz_init_set_ui(e, PUBLIC_EXPONENT);
  
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
  
  mpz_clears(p, q, lambda_n, gcd, p_minus_1, q_minus_1, NULL);
}

void rsa_encrypt(mpz_t c, const mpz_t m, const mpz_t e, const mpz_t n) {
  if (mpz_cmp(m, n) >= 0) // error
    exit(EXIT_FAILURE);
  mpz_powm(c, m, e, n); 
}

void rsa_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t n) {
  mpz_powm(m, c, d, n);
}


