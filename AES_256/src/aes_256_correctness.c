#include <stdio.h>
#include "aes_256.h"

int main() {

    /*
     * Test vector from FIPS 197 for AES-256
     * PLAINTEXT:  00112233445566778899aabbccddeeff
     * KEY:        000102030405060708090a0b0c0d0e0f
     *             101112131415161718191a1b1c1d1e1f
     * CIPHERTEXT: 8ea2b7ca516745bfeafc49904b496089
     */

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

    u8 expected_cipher[AES_BLOCK_SIZE] = {
        0x8e, 0xa2, 0xb7, 0xca,
        0x51, 0x67, 0x45, 0xbf,
        0xea, 0xfc, 0x49, 0x90,
        0x4b, 0x49, 0x60, 0x89
    };

    u8 round_key[AES256_EXPANDED_KEY_SIZE];
    key_expand(key, round_key);

    state_t state;
    load_state(state, plain_text);

    u8 cipher_text[AES_BLOCK_SIZE];
    encrypt_block(state, round_key);
    store_state(state, cipher_text);

    int error = 0;

    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        if (cipher_text[i] != expected_cipher[i]) {
            error = 1;
            break;
        }
    }

    if (error == 0) {
        printf("PASS: AES-256 known-answer test\n");
        return 0;
    }

    fprintf(stderr, "FAIL: AES-256 known-answer test\n");
    return 1;
}