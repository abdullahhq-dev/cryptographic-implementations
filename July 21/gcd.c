#include <stdio.h>

unsigned int gcd(unsigned int a, unsigned int b) {
  if (b == 0) return a;
  unsigned int rem = a % b;
  while (rem != 0) {
    a = b;
    b = rem;
    rem = a % b;
  }
  return b;
}

int main() {
  printf("%u\n",gcd(18,12));
  return 0;
}
