#include "dilithium_ntt.h"

const int64_t Q = 8380417;
const int64_t ZETA = 1753;

static uint8_t bit_rev(uint8_t m) {
    uint8_t rev = 0;
    for (int i = 0; i < 8; i++) {
        if ((m >> i) & 1) {
            rev |= (1 << (7 - i));
        }
    }
    return rev;
}

static int64_t power_mod(int64_t base, int64_t exp, int64_t mod) {
    int64_t res = 1;
    base = base % mod;
    while(exp > 0) {
        if (exp % 2 == 1) {
            res = (res * base) % mod;
        }
        base =(base * base) % mod;
        exp /= 2;
    }
    return res;
}

void gen_zetas(int64_t zetas[DILITHIUM_N]) {
    uint8_t rev_m;

    for (int m = 0; m < DILITHIUM_N; m++) {
        rev_m = bit_rev((uint8_t) m);
        zetas[m] = power_mod(ZETA, rev_m, Q);
    }
}

void dilithium_ntt(int64_t poly[DILITHIUM_N], const int64_t zetas[DILITHIUM_N]) {
    uint8_t m = 0;
    int64_t z, t;
    int len = 128;
    int start;

    while(len >= 1) {
        start = 0;
        while(start < DILITHIUM_N) {
            m++;
            z = zetas[m];
            for (int j = start; j < start + len; j++) {
                t = (z * poly[j + len]) % Q;
                poly[j + len] = (poly[j] - t + Q) % Q;
                poly[j] = (poly[j] + t) % Q;
            }
            start += 2 * len;
        }
        len /= 2;
    }

}

void dilithium_intt(int64_t poly[DILITHIUM_N], const int64_t zetas[DILITHIUM_N]) {
    int k = DILITHIUM_N, len = 1; // len is the degree of the polynomials in the current level
    while(len < DILITHIUM_N) {

        int start = 0;

        while (start < DILITHIUM_N) {

            k--;
    
            int64_t z = (Q - zetas[k]) % Q, t;
            for (int j = start; j < start + len; j++) {
                t = poly[j];
                poly[j] = (t + poly[j + len]) % Q;
                int64_t diff = (t - poly[j + len] + Q) % Q;
                poly[j + len] = (diff * z) % Q;
            }
            start = start + 2 * len;
        }

        len = len * 2;
    }

    // Multiply every coefficient by 1/n mod Q .
    // for dilithium ( q = 8380417), the inverse of DILITHIUM_N is 8347681
    const int64_t F = 8347681;
    for (int i = 0; i < DILITHIUM_N; i++) {
        poly[i] = (poly[i] * F) % Q;
    }
}