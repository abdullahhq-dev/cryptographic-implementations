#include<stdio.h>
#include <stdint.h>
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

void QR(u32 state[16], u8 a, u8 b, u8 c, u8 d);
void inner_block( u32 state[16]);
void chacha20_block(const u32 input_state[16], u8 output_state[64]);
void chacha20_init_state( u32 state[16], const u8 key[32], const u32 counter, const u8 nonce[12]);
void chacha20_encrypt(u32 init_state[16],const u8 plain_text[], u8 cipher_text[], size_t length);
