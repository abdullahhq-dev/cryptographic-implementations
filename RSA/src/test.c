#include "rsa.h"

int main() {

  gmp_randstate_t state;
  rsa_rand_init(state);
  
  mpz_t n, e, d, p, q, dP, dQ, q_inv;
  mpz_inits(n, e, d, p, q, dP, dQ, q_inv, NULL);
  rsa_keygen(state, n, e, d, p, q, dP, dQ, q_inv);
  
  mpz_t m, c, decrypted;
  mpz_inits(m, c, decrypted, NULL);
  
  mpz_set_ui(m, 1234567890);
  rsa_encrypt(c, m, e, n);
  rsa_decrypt(decrypted, c, n, d);
  
  gmp_printf("message : %Zd\n", m);

  gmp_printf("decrypted message by textbook rsa: %Zd\n", decrypted);
  
  rsa_decrypt_crt(decrypted, c, n, p, q, dP, dQ, q_inv); 
  gmp_printf("decrypted message by normal crt rsa: %Zd\n", decrypted);
  
  mpz_clears(n, e, d, p, q, dP, dQ, q_inv, m, c, decrypted, NULL);
  gmp_randclear(state);
  
return 0;
}
