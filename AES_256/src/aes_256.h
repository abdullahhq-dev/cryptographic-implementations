#ifndef AES_H
#define AES_H

#include <stdint.h>
enum {
    AES_BLOCK_SIZE = 16,
    AES256_KEY_SIZE = 32,
    AES256_NK = 8,
    AES256_NR = 14,
    AES256_EXPANDED_KEY_SIZE = 240
};

typedef uint8_t u8;
typedef uint8_t state_t[4][4];

void load_state(state_t state, const u8 input[AES_BLOCK_SIZE]);
void store_state(const state_t state, u8 output[AES_BLOCK_SIZE]);

void encrypt_block(state_t state, const u8 round_key[AES256_EXPANDED_KEY_SIZE]);
void key_expand(const u8 key[AES256_KEY_SIZE], u8 round_key[AES256_EXPANDED_KEY_SIZE]);

#endif
