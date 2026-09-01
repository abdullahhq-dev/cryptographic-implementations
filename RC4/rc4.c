/* Abdulla Abdul Salam - CRS2501
    RC4 Impelmentation
*/
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

#define ITERATIONS 1000000ULL
typedef uint8_t u8;

static inline void swap(u8 *a, u8 *b) {
  u8 temp = *a;
  *a = *b;
  *b = temp;
}

void key_init( const u8 *key, const size_t key_len, u8 S[256]) {
  for (int i = 0; i < 256; i++) {
    S[i] = i;
  }
  
  u8 j = 0;
  for (int i = 0; i < 256; i++) {
    j += S[i] + key [ i % key_len ];
    swap(&S[i] , &S[j]);
  }
  
}

u8 keystream(u8 S[256], u8 *i, u8 *j) {
  u8 ii = *i + 1;
  u8 jj = *j + S[ii];
  swap(&S[ii], &S[jj]);
  
  *i =ii; *j =jj;
  
  return S[(u8)(S[ii] + S[jj])];
}

int main() {
  u8 i = 0, j = 0;
  u8 S[256];
  u8 key[] = {0x01,0x02,0x03,0x04,0x05};

  uint64_t start_cycles;
  uint64_t end_cycles;
  uint64_t total_cycles;
  

  volatile u8 checksum = 0;
  unsigned int cpu_id;
  // Measuring Number of cycles required for Key Intialization
  start_cycles = __rdtscp(&cpu_id);
  
  for (uint64_t m = 0; m < ITERATIONS; m++) {
    key_init(key, sizeof(key), S);
    checksum ^= S[0];
  }
  end_cycles = __rdtscp(&cpu_id);
  total_cycles = end_cycles - start_cycles;  
  
  printf("Avg no. of cycles for key init          : %.2f\n", (double)total_cycles / ITERATIONS);

  //Testing PRGA
  start_cycles = __rdtscp(&cpu_id);
  
  for (uint64_t m = 0; m < ITERATIONS; m++) {
    checksum ^= keystream(S, &i, &j);
  }

  end_cycles = __rdtscp(&cpu_id);
  total_cycles = end_cycles - start_cycles;
  
  double cycles_per_byte = (double) total_cycles / ITERATIONS;
  
  printf("Cycles / Byte for key_stream generation : %.4f\n", cycles_per_byte);
  printf("Checksum :%d\n", checksum);
  return 0;
}

/* --Results--

-O0

Avg no. of cycles for key init          : 10803.54
Cycles / Byte for key_stream generation : 33.6290

-O2

Avg no. of cycles for key init          : 7172.27
Cycles / Byte for key_stream generation : 6.3412

-O3 -march=native

Avg no. of cycles for key init          : 2052.76
Cycles / Byte for key_stream generation : 6.3344

  --Analysis--
  
The PRGA outputs 1 byte of keystreamp per iteration. for each iteration the algorithm performs approximately
3 additions
9 memory access 
total ~ 12 operations for each iteration
The PRGA requires approximately 6-7 cycles for one iteration under -O3 -march=native


The Key Scheduling Algorithm KSA is executed only once for a given secret key, irrespective of the message length. Therefore, its performance is reported as the total number of cycles required for one key initialization, rather than cycles per byte.

The Key Scheduling Algorithm (KSA) executes exactly 256 iterations. Each iteration performs:

One lookup from the state array S.
One lookup from the secret key.
One addition to update the index j.
One swap of two elements in S.
Loop increment and branch.

The KSA requires approximately 2053 cycles for 256 iterations under -O3 -march=native, corresponding to roughly 2052.76 / 256 ≈8.0 cycles per iteration.
Similarly, the PRGA requires approximately 6.33 cycles to generate one keystream byte. Since both algorithms perform similar operations (table lookups, additions, and swaps), these values are consistent with the expected computational cost. The PRGA achieves a slightly lower cycle count per iteration because it avoids the additional secret-key lookup performed by the KSA.
*/


