#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

unsigned int gcd(unsigned int a, unsigned int b) {
  if (b == 0) return a;
  unsigned int rem = a % b;
  while (rem != 0) {
    a = b;
    b = rem;
    rem = a % b;
  }
  return b;
}

int main() {
  
  unsigned int A[10000];
  unsigned int B[10000];

  for (int i = 0; i < 10000; i++) {
      A[i] = rand() % 10000;
      B[i] = rand() % 10000;
  }

  struct timespec start_time, end_time;
  uint64_t total_time_ns;
  
  volatile unsigned int checksum = 0;
  
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  for (int i = 0; i < 10000; i++) {
    checksum ^= gcd(A[i],B[i]);
  }
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  total_time_ns = (uint64_t)(end_time.tv_sec - start_time.tv_sec) * 1000000000ULL + (uint64_t)(end_time.tv_nsec - start_time.tv_nsec);
  
 double ns_per_iteration = (double) total_time_ns / 10000;
  
  printf("Checksum %u\n", checksum);
  printf("Iterations            : 10,000\n");
  printf("Total Time            : %lu ns\n", total_time_ns);
  printf("Average execution Time     : %f ns\n", ns_per_iteration);
  return 0;
}
