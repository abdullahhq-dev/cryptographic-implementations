#include "chacha.h"

static u32 ROTL(u32 x, u8 n) { //Rotating the bits left by n positions
  return ( x << n | x >> (32 - n));
}

static u32 load32_le(const u8 b[4]) {
  return ((u32) b[0]) | ((u32) b[1] << 8) | ((u32) b[2] << 16) | ((u32) b[3] << 24);
}

static void store32_le(const u32 input, u8 output[4]) {
  for (int i = 0; i < 4; i++) {
    output[i] = (u8) ( input >> (8 * i));
  }
}

void chacha20_init_state( u32 state[16], const u8 key[32], const u32 counter, const u8 nonce[12]) {
  state[0] = 0x61707865;
  state[1] = 0x3320646e;
  state[2] = 0x79622d32;
  state[3] = 0x6b206574;
  
  for(int i = 0; i < 8; i++) {
    state[i+4] = load32_le( key + 4 * i );
  }
  
  state[12] = counter;
  
  state[13] = load32_le( &nonce[0] );
  state[14] = load32_le( &nonce[4] );
  state[15] = load32_le( &nonce[8] );
}

void QR(u32 state[16], u8 a, u8 b, u8 c, u8 d) {
  state[a] += state[b]; state[d] ^= state[a]; state[d] = ROTL(state[d],16);
  state[c] += state[d]; state[b] ^= state[c]; state[b] = ROTL(state[b],12);
  state[a] += state[b]; state[d] ^= state[a]; state[d] = ROTL(state[d],8);
  state[c] += state[d]; state[b] ^= state[c]; state[b] = ROTL(state[b],7);
}

void inner_block( u32 state[16]) {
  QR(state, 0, 4, 8,12);
  QR(state, 1, 5, 9,13);
  QR(state, 2, 6, 10,14);
  QR(state, 3, 7, 11,15);
  QR(state, 0, 5, 10,15);
  QR(state, 1, 6, 11,12);
  QR(state, 2, 7, 8,13);
  QR(state, 3, 4, 9,14);
}

void chacha20_block(const u32 input_state[16], u8 output_state[64]) {
  u32 working_state[16];  
  for (int i = 0; i < 16; i++) {
    working_state[i] = input_state[i];
  }
  for (int i=0; i < 10; i++) {
    inner_block(working_state);
  }
  for (int i = 0; i < 16; i++) {
    working_state[i] += input_state[i];
  }
  
  for (int i = 0; i < 16; i++) {
    store32_le( working_state[i], output_state + 4 * i );
    }
}

void chacha20_encrypt(u32 init_state[16], const u8 plain_text[], u8 cipher_text[], size_t length) {
  u8 key_stream[64];
  for ( int i = 0; i < length / 64; i++) {    // generating the keyblock
    chacha20_block(init_state , key_stream); init_state[12]++;
    for ( int j = 0; j < 64; j++) {
      cipher_text[64 * i + j] = plain_text[64 * i + j] ^ key_stream[j];
    }
  }
  
  if (length % 64 != 0) {
    u32 fullblocks = length / 64;
    chacha20_block(init_state , key_stream); init_state[12]++;
    for ( int i = 0; i < length % 64; i++) {
     cipher_text[fullblocks * 64 + i] = plain_text[fullblocks * 64 + i ] ^ key_stream[i];
    }
  }
}

