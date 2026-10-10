# Dilithium NTT in C

An implementation of the Number Theoretic Transform (NTT) and inverse NTT used in Dilithium/ML-DSA, written in C.

The implementation follows the NTT structure used by Dilithium, with:

- Polynomial size: `256`
- Modulus: `q = 8380417`
- Root of unity: `1753`
- Forward NTT
- Inverse NTT
- Zeta table generation

## Files

```text
src
├── dilithium_ntt.c
├── dilithium_ntt.h
└── test_ntt.c
```

`dilithium_ntt.c` contains the implementation, `dilithium_ntt.h` contains the public interface, and `test_ntt.c` contains the tests.

## Testing

The test program checks that applying the forward NTT followed by the inverse NTT gives back the original polynomial.

It includes tests for:

- Zero polynomial
- Monomials
- Boundary coefficients
- A patterned polynomial
- A pseudo-random polynomial

The tests also check that the NTT output coefficients remain in the expected range.

Run with:

```bash
gcc -Wall -Wextra -Wpedantic dilithium_ntt.c test_ntt.c -o test_ntt
./test_ntt
```

Expected output ends with:

```text
All tests passed.
```
