/*
 * Test code written with AI assistance.
 * I reviewed and tested the code.
 */

#include <inttypes.h>
#include <stdio.h>

#include "dilithium_ntt.h"


// Check whether INTT(NTT(poly)) recovers the original polynomial.
static int check_round_trip(
    const char *name,
    const int64_t input[DILITHIUM_N],
    const int64_t zetas[DILITHIUM_N]
) {
    int64_t poly[DILITHIUM_N];

    // Copy the input because the NTT functions modify the polynomial in place.
    for (int i = 0; i < DILITHIUM_N; i++) {
        poly[i] = input[i];
    }

    // Apply the forward NTT.
    dilithium_ntt(poly, zetas);

    // Check that the NTT output contains valid residues modulo Q.
    for (int i = 0; i < DILITHIUM_N; i++) {
        if (poly[i] < 0 || poly[i] >= Q) {
            fprintf(
                stderr,
                "FAIL: %s: invalid NTT coefficient at index %d: %" PRId64 "\n",
                name, i, poly[i]
            );
            return 0;
        }
    }

    // Apply the inverse NTT.
    dilithium_intt(poly, zetas);

    // Compare the recovered polynomial with the original.
    for (int i = 0; i < DILITHIUM_N; i++) {
        if (poly[i] != input[i]) {
            fprintf(
                stderr,
                "FAIL: %s: mismatch at index %d "
                "(expected %" PRId64 ", got %" PRId64 ")\n",
                name, i, input[i], poly[i]
            );
            return 0;
        }
    }

    return 1;
}


// Run one test and print its result.
static int run_test(
    const char *name,
    const int64_t input[DILITHIUM_N],
    const int64_t zetas[DILITHIUM_N]
) {
    if (!check_round_trip(name, input, zetas)) {
        return 0;
    }

    printf("PASS: %s\n", name);
    return 1;
}


int main(void) {
    int64_t zetas[DILITHIUM_N];

    // Generate the zeta table once.
    gen_zetas(zetas);

    int failures = 0;

    // Test 1: Zero polynomial.
    int64_t zero[DILITHIUM_N] = {0};

    failures += !run_test(
        "Zero polynomial", zero, zetas
    );


    // Test 2: A single nonzero coefficient at index 1.
    int64_t monomial[DILITHIUM_N] = {0};
    monomial[1] = 1;

    failures += !run_test(
        "Monomial at index 1", monomial, zetas
    );


    // Test 3: A single nonzero coefficient at the last index.
    int64_t highest_monomial[DILITHIUM_N] = {0};
    highest_monomial[DILITHIUM_N - 1] = 1;

    failures += !run_test(
        "Monomial at highest index", highest_monomial, zetas
    );


    // Test 4: Boundary values, including Q-1 and Q-2.
    int64_t boundary[DILITHIUM_N];

    for (int i = 0; i < DILITHIUM_N; i++) {
        boundary[i] = (i % 2 == 0) ? Q - 1 : 0;
    }

    boundary[1] = 1;
    boundary[2] = Q - 2;
    boundary[3] = Q / 2;

    failures += !run_test(
        "Boundary coefficients", boundary, zetas
    );


    // Test 5: A deterministic polynomial with varied coefficients.
    int64_t patterned[DILITHIUM_N];

    for (int i = 0; i < DILITHIUM_N; i++) {
        patterned[i] =
            ((int64_t)i * 1234567
             + (int64_t)i * i * 89
             + 42) % Q;
    }

    failures += !run_test(
        "Patterned polynomial", patterned, zetas
    );


    // Test 6: Deterministic pseudo-random coefficients.
    int64_t pseudo_random[DILITHIUM_N];
    uint32_t state = 0xC0FFEEu;

    for (int i = 0; i < DILITHIUM_N; i++) {
        // Xorshift32 pseudo-random number generator.
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;

        pseudo_random[i] = state % Q;
    }

    failures += !run_test(
        "Pseudo-random polynomial", pseudo_random, zetas
    );


    // Report the overall result.
    if (failures == 0) {
        printf("\nAll tests passed.\n");
        return 0;
    }

    fprintf(stderr, "\n%d test(s) failed.\n", failures);
    return 1;
}