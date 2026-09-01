#include "rsa.h"
#include <stdio.h>
#include <x86intrin.h>  
#include <stdint.h>

#define ITERATIONS 10000

int main() {

  gmp_randstate_t state;
  rsa_rand_init(state);
  
  mpz_t n, e, d, p, q, dP, dQ, q_inv;
  mpz_inits(n, e, d, p, q, dP, dQ, q_inv, NULL);
  rsa_keygen(state, n, e, d, p, q, dP, dQ, q_inv);
  
  mpz_t m, c, decrypted;
  mpz_inits(m, c, decrypted, NULL);
  
  mpz_urandomb(m, state, 2047);
  struct timespec start_time, end_time;
  uint64_t total_time_ns;
  
  uint32_t cpu_id;
  uint64_t start_cycles, end_cycles, total_cycles;
  
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  start_cycles = __rdtscp(&cpu_id);
  
  for (int i = 0; i < ITERATIONS; i++) {
   rsa_encrypt(c, m, e, n);  
  }
  
  end_cycles = __rdtscp(&cpu_id);
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  
  total_cycles = end_cycles - start_cycles;
  // Cycles Per Iteration
  double cycles_per_iteration = (double)total_cycles / ITERATIONS;
  
  total_time_ns = (uint64_t)(end_time.tv_sec - start_time.tv_sec) * 1000000000ULL + (uint64_t)(end_time.tv_nsec - start_time.tv_nsec);
  
  double ns_per_iteration = (double) total_time_ns / ITERATIONS;
  printf("\nRSA_ENCRYPT\n");
  printf("Iterations              : %u\n", ITERATIONS);
  printf("Total Cycles            : %.2f M\n",
         (double)total_cycles / 1e6);
  printf("Cycles / Iteration      : %.2f k\n",
         cycles_per_iteration / 1e3);
  printf("Total Time              : %.2f ms\n",
         (double)total_time_ns / 1e6);
  printf("Average Time / Iteration: %.2f us\n",
         ns_per_iteration / 1e3);
  
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  start_cycles = __rdtscp(&cpu_id);
  
  for (int i = 0; i < ITERATIONS; i++) {
    rsa_decrypt_crt(decrypted, c, n, p, q, dP, dQ, q_inv); 
  }
  
  end_cycles = __rdtscp(&cpu_id);
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  
  total_cycles = end_cycles - start_cycles;
  // Cycles Per Iteration
  cycles_per_iteration = (double)total_cycles / ITERATIONS;
  
  total_time_ns = (uint64_t)(end_time.tv_sec - start_time.tv_sec) * 1000000000ULL + (uint64_t)(end_time.tv_nsec - start_time.tv_nsec);
  
  ns_per_iteration = (double) total_time_ns / ITERATIONS;
  
  printf("\nRSA_CRT_DECRYPT\n");
  printf("Iterations              : %u\n", ITERATIONS);
  printf("Total Cycles            : %.2f M\n",
         (double)total_cycles / 1e6);
  printf("Cycles / Iteration      : %.3f M\n",
         cycles_per_iteration / 1e6);
  printf("Total Time              : %.3f s\n",
         (double)total_time_ns / 1e9);
  printf("Average Time / Iteration: %.3f ms\n",
         ns_per_iteration / 1e6);
         
         
  // normal decryption
  
    clock_gettime(CLOCK_MONOTONIC, &start_time);
  start_cycles = __rdtscp(&cpu_id);
  
  for (int i = 0; i < ITERATIONS; i++) {
    rsa_decrypt(decrypted, c, n, d); 
  }
  
  end_cycles = __rdtscp(&cpu_id);
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  
  total_cycles = end_cycles - start_cycles;
  // Cycles Per Iteration
  cycles_per_iteration = (double)total_cycles / ITERATIONS;
  
  total_time_ns = (uint64_t)(end_time.tv_sec - start_time.tv_sec) * 1000000000ULL + (uint64_t)(end_time.tv_nsec - start_time.tv_nsec);
  
  ns_per_iteration = (double) total_time_ns / ITERATIONS;
  
  printf("\nRSA_DECRYPT\n");
  printf("Iterations              : %u\n", ITERATIONS);
  printf("Total Cycles            : %.2f M\n",
         (double)total_cycles / 1e6);
  printf("Cycles / Iteration      : %.3f M\n",
         cycles_per_iteration / 1e6);
  printf("Total Time              : %.3f s\n",
         (double)total_time_ns / 1e9);
  printf("Average Time / Iteration: %.3f ms\n",
         ns_per_iteration / 1e6);
  
  mpz_clears(n, e, d, p, q, dP, dQ, q_inv, m, c, decrypted, NULL);
  gmp_randclear(state);
  
return 0;
}



