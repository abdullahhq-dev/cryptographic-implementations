#ifndef RSA_H
#define RSA_H

#include <time.h>
#include <gmp.h>

#define PRIME_SIZE 1024
#define PUBLIC_EXPONENT 65537

void rsa_rand_init(gmp_randstate_t state);

void rsa_generate_prime(mpz_t p,
                        gmp_randstate_t state,
                        const mp_bitcnt_t bits);

void rsa_keygen(mpz_t n,
                mpz_t e,
                mpz_t d,
                gmp_randstate_t state);

void rsa_encrypt(mpz_t c,
                 const mpz_t m,
                 const mpz_t e,
                 const mpz_t n);

void rsa_decrypt(mpz_t m,
                 const mpz_t c,
                 const mpz_t d,
                 const mpz_t n);

#endif /* RSA_H */
