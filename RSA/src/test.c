#include "rsa.h"

int main() {

  gmp_randstate_t state;
  rsa_rand_init(state);
  
  mpz_t e, n, d;
  mpz_inits(n, e, d, NULL);
  rsa_keygen(n, e, d, state);
  
  mpz_t m, c, decrypted;
  mpz_inits(m, c, decrypted, NULL);
  
  mpz_set_ui(m, 1382091049801294);
  rsa_encrypt(c, m, e, n);
  rsa_decrypt(decrypted, c, d, n);
  
  gmp_printf("message : %Zd\n", m);
  gmp_printf("cipher_text : %Zd\n", c);
  gmp_printf("decrypted message : %Zd\n", decrypted);
  
  mpz_clears(n, d, m, c, decrypted, NULL);
  gmp_randclear(state);
  
return 0;
}
