#ifndef DILITHIUM_NTT_H
#define DILITHIUM_NTT_H

#include <stdint.h>

#define DILITHIUM_N 256

extern const int64_t Q;

void gen_zetas(int64_t zetas[DILITHIUM_N]);

void dilithium_ntt(
    int64_t poly[DILITHIUM_N],
    const int64_t zetas[DILITHIUM_N]
);

void dilithium_intt(
    int64_t poly[DILITHIUM_N],
    const int64_t zetas[DILITHIUM_N]
);

#endif