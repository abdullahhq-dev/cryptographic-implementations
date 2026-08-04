#include <stdio.h>
#include <time.h>
#include <x86intrin.h>
#include "aes_256.h"

typedef uint32_t u32;
typedef uint64_t u64;

#define ITERATIONS 100000
#define MSG_SIZE 16 //BYTES
int main() {
u8 plain_text[AES_BLOCK_SIZE] = {
    0x00, 0x11, 0x22, 0x33,
    0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb,
    0xcc, 0xdd, 0xee, 0xff
};

u8 key[AES256_KEY_SIZE] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b,
    0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b,
    0x1c, 0x1d, 0x1e, 0x1f
};
                
  u8 round_key[AES256_EXPANDED_KEY_SIZE];
  key_expand(key, round_key);
  
  
  
  
  
  //Starting measuring
  struct timespec start_time, end_time;
  u64 total_time_ns;
  u64 total_bytes = ITERATIONS * MSG_SIZE;
  
  u32 cpu_id;
  u64 start_cycles, end_cycles, total_cycles;

  state_t state;
  volatile u8 checksum = 0;
  
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  start_cycles = __rdtscp(&cpu_id);
  
  for (int i = 0; i < ITERATIONS; i++) {
      load_state(state, plain_text);
      encrypt_block(state, round_key);
      checksum ^= state[0][0];
  }
    
  end_cycles = __rdtscp(&cpu_id);
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  
  printf("Checksum: %02x\n", checksum);
    
  total_cycles = end_cycles - start_cycles;
  // Cycles Per Iteration
  double cycles_per_iteration = (double)total_cycles / ITERATIONS;
  //Cycles Per Byte
  double cycles_per_byte = (double) total_cycles / total_bytes;
  
  total_time_ns = (u64)(end_time.tv_sec - start_time.tv_sec) * 1000000000ULL + (u64)(end_time.tv_nsec - start_time.tv_nsec);
  
  // average execution time for 64 bytes long message text
  double ns_per_iteration = (double) total_time_ns / ITERATIONS;
  // Time per byte
  double ns_per_byte = (double) total_time_ns / ( total_bytes );
  
  printf("Iterations            : %u\n", ITERATIONS);
  printf("Total Cycles           : %lu\n", total_cycles);
  printf("Cycles / Iteration    : %.2f\n", cycles_per_iteration);
  printf("Cycles / Byte         : %.4f\n", cycles_per_byte);
  printf("Total Time            : %lu ns\n", total_time_ns);
  printf("Average Time / Iteration     : %.2f ns\n", ns_per_iteration);
  printf("Average Time / Byte    : %.2f ns\n", ns_per_byte);  
  
  u8 cipher_text[AES_BLOCK_SIZE];
  store_state(state, cipher_text);
  printf("Cipher text : \n");
  for ( int i = 0; i < AES_BLOCK_SIZE; i++) {
    printf("%02x", cipher_text[i]);
  }
  printf("\n");
  return 0;
}

  

  

