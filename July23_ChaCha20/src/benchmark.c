#include <stdio.h>
#include "chacha.h"

#include <stdint.h>
#include <time.h>
#include <x86intrin.h>  

#define ITERATIONS 1000000ULL
#define MSG_SIZE 64 // IN BYTES

int main() {

  u8 key[32] = {
      0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
      0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
      0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
      0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f
  };
  
  u8 nonce[12] = {
      0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x4a,
      0x00,0x00,0x00,0x00
  };
  
  u32 counter = 1;
  
  u32 init_state[16];
  
  chacha20_init_state( init_state, key, counter, nonce);
  
  u8 plain_text[MSG_SIZE] = {0};
  size_t len = MSG_SIZE;
  
  u8 cipher_text[MSG_SIZE];
  
  struct timespec start_time, end_time;
  u64 total_time_ns;
  
  u32 cpu_id;
  u64 start_cycles, end_cycles, total_cycles;

  u64 total_bytes = ITERATIONS * MSG_SIZE;
  
  volatile u8 checksum = 0;
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  start_cycles = __rdtscp(&cpu_id);
  
  for (int i = 0; i < ITERATIONS; i++) {
    chacha20_encrypt( init_state, plain_text, cipher_text, len) ;
    checksum ^= cipher_text[0];
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
  
  printf("Iterations            : %llu\n", ITERATIONS);
  printf("Total Cycles           : %lu\n", total_cycles);
  printf("Cycles / Iteration    : %.2f\n", cycles_per_iteration);
  printf("Cycles / Byte         : %.4f\n", cycles_per_byte);
  printf("Total Time            : %lu ns\n", total_time_ns);
  printf("Average Time / Iteration     : %.2f ns\n", ns_per_iteration);
  printf("Average Time / Byte    : %.2f ns\n", ns_per_byte);  
  
}

