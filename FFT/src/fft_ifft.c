#include <stdio.h>
#include <math.h>
#include <complex.h>

#define PI 3.14159265358979323846

// We assume n to be a power of 2.
void fft(double complex input[],double complex output[], int n) {
    complex double odd[n/2], even[n/2], even_fft[n/2], odd_fft[n/2];
    int k;
    if (n == 1) {
        output[0] = input[0];
        return;
    }
    for ( k = 0; k < n/2; k++) {
        even[k] = input[2 * k];
        odd[k] = input[2 * k + 1];
    }
    fft(even,even_fft,n/2);
    fft(odd,odd_fft,n/2);

    double complex omega = cexp(2.0 * I * PI / n);
    double complex w = 1.0;

    for (k = 0; k < n/2; k++) {
        odd_fft[k] *= w;
        w *= omega;
    }

    for (k = 0; k < n/2; k++) {
        output[k] = even_fft[k] + odd_fft[k];
        output[k + n/2] = even_fft[k] - odd_fft[k];
    }
}

void ifft_helper(double complex input[],double complex output[], int n) {
    complex double odd[n/2], even[n/2], even_ifft[n/2], odd_ifft[n/2];
    int k;
    if (n == 1) {
        output[0] = input[0];
        return;
    }
    for ( k = 0; k < n/2; k++) {
        even[k] = input[2 * k];
        odd[k] = input[2 * k + 1];
    }
    ifft_helper(even,even_ifft,n/2);
    ifft_helper(odd,odd_ifft,n/2);

    double complex omega = cexp(-2.0 * I * PI / n);
    double complex w = 1.0;

    for (k = 0; k < n/2; k++) {
        odd_ifft[k] *= w;
        w *= omega;
    }

    for (k = 0; k < n/2; k++) {
        output[k] = even_ifft[k] + odd_ifft[k];
        output[k + n/2] = even_ifft[k] - odd_ifft[k];
    }
}
void ifft(double complex input[],double complex output[], int n) {
    ifft_helper(input, output,n);
    for (int k = 0; k < n; k++) {
        output[k] /= n;
    }

}

int main() {
    double complex input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    double complex output[8];
    fft(input, output, 8);
    for (int i = 0; i < 8; i++) {
        printf(" %.2f + %.2fi\n", creal(output[i]), cimag(output[i]));
    }
    ifft(output, input, 8);
    for (int i = 0; i < 8; i++) {
        printf(" %.2f + %.2fi\n", creal(input[i]), cimag(input[i]));
    }
}