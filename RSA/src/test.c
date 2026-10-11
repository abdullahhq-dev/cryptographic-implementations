#include <stdio.h>
#include "rsa.h"

int main(void) {
    gmp_randstate_t state;
    rsa_rand_init(state);

    mpz_t n, e, d, p, q, dP, dQ, q_inv;
    mpz_inits(n, e, d, p, q, dP, dQ, q_inv, NULL);

    rsa_keygen(state, n, e, d, p, q, dP, dQ, q_inv);

    mpz_t m, c, decrypted, crt_decrypted;
    mpz_inits(m, c, decrypted, crt_decrypted, NULL);

    mpz_set_ui(m, 1234567890);

    rsa_encrypt(c, m, e, n);

    rsa_decrypt(decrypted, c, n, d);
    rsa_decrypt_crt(crt_decrypted, c, n, p, q, dP, dQ, q_inv);

    int normal_ok = (mpz_cmp(m, decrypted) == 0);
    int crt_ok = (mpz_cmp(m, crt_decrypted) == 0);

    printf("%s: RSA decryption\n", normal_ok ? "PASS" : "FAIL");
    printf("%s: RSA-CRT decryption\n", crt_ok ? "PASS" : "FAIL");

    mpz_clears(n, e, d, p, q, dP, dQ, q_inv,
               m, c, decrypted, crt_decrypted, NULL);
    gmp_randclear(state);

    return (normal_ok && crt_ok) ? 0 : 1;
}