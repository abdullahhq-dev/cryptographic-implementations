/* The code below is generated using Gemini AI by combining the codes I had written for all 3 sorting algorithms last year as part of the intro to c course. The Execution times are calculated using the same method I wrote for the gcd program.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// ==========================================
// 1. Heap Sort Implementation
// ==========================================
void heapify(int A[], int n, int i){
    int largest = i, l = 2*i+1, r = 2*i+2;
    if(l<n && A[l]>A[largest]) largest = l;
    if(r<n && A[r]>A[largest]) largest = r;
    if(largest!=i){
        int t = A[i]; A[i]=A[largest]; A[largest]=t;
        heapify(A,n,largest);
    }
}

void heapSort(int A[], int n){
    for(int i=n/2-1;i>=0;i--) heapify(A,n,i);
    for(int i=n-1;i>0;i--){
        int t=A[0]; A[0]=A[i]; A[i]=t;
        heapify(A,i,0);
    }
}

// ==========================================
// 2. Merge Sort Implementation
// ==========================================
void merge(int A[], int p, int q, int r) {
    int n1 = q - p + 1, n2 = r - q; 
    int B[n1 + n2];
    int i = 0, j = 0, k = 0;

    while (i < n1 && j < n2) {
        if (A[p + i] <= A [q+1 + j]) {
            B[k] = A[p + i];
            i++;
        } else {
            B[k] = A[q+1 + j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        B[k] = A[p + i];
        i++;
        k++; 
    }
    while (j < n2) {
        B[k] = A[q+1 + j];
        j++;
        k++;
    } 
    for (int i = 0; i < n1 + n2; i++) {
        A[p + i] = B[i];
    } 
}

void mergeSort(int A[], int p, int r) {
    if (p < r) {
        int q = (p + r) / 2;
        mergeSort(A, p, q);
        mergeSort(A, q + 1, r);
        merge(A, p, q, r);
    }
}

// ==========================================
// 3. Quick Sort Implementation
// ==========================================
int partition(int A[], int p, int r) {
    int temp, i = p - 1, x = A[r];
    for (int j = p; j < r; j++) {
        if (A[j] <= x) {
            i++;
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;   
        }
    }
    temp = A[i+1];
    A[i+1] = A[r];
    A[r] = temp; 
    return i+1;
}

void quickSort(int A[], int p, int r) {
    if(p < r) {
        int q = partition(A, p, r);
        quickSort(A, p, q - 1);
        quickSort(A, q + 1, r);
    }
}

// ==========================================
// Measurement and Execution
// ==========================================
int main() {
    srand(time(NULL));
    
    int sizes[] = {10, 100, 1000};
    uint64_t heap_times[3];
    uint64_t merge_times[3];
    uint64_t quick_times[3];

    struct timespec start_time, end_time;

    for (int i = 0; i < 3; i++) {
        int n = sizes[i];
        int *A = (int*) malloc(n * sizeof(int));

        // --- 1. Measure Heap Sort manually ---
        for (int j = 0; j < n; j++) A[j] = 50 - rand() % 100;
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        heapSort(A, n);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        heap_times[i] = (uint64_t)(end_time.tv_sec - start_time.tv_sec) * 1000000000ULL + (uint64_t)(end_time.tv_nsec - start_time.tv_nsec);

        // --- 2. Measure Merge Sort manually ---
        for (int j = 0; j < n; j++) A[j] = 50 - rand() % 100;
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        mergeSort(A, 0, n - 1);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        merge_times[i] = (uint64_t)(end_time.tv_sec - start_time.tv_sec) * 1000000000ULL + (uint64_t)(end_time.tv_nsec - start_time.tv_nsec);

        // --- 3. Measure Quick Sort manually ---
        for (int j = 0; j < n; j++) A[j] = 50 - rand() % 100;
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        quickSort(A, 0, n - 1);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        quick_times[i] = (uint64_t)(end_time.tv_sec - start_time.tv_sec) * 1000000000ULL + (uint64_t)(end_time.tv_nsec - start_time.tv_nsec);

        free(A);
    }

    // Print all outputs at the end
    printf("========================================================\n");
    printf("   Sorting Algorithm Execution Time (Single Run)\n");
    printf("========================================================\n");
    printf("%-10s | %-12s | %-12s | %-12s\n", "Size", "Heap Sort", "Merge Sort", "Quick Sort");
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < 3; i++) {
        printf("%-10d | %-9lu ns | %-9lu ns | %-9lu ns\n", 
               sizes[i], heap_times[i], merge_times[i], quick_times[i]);
    }
    printf("========================================================\n");

    return 0;
}

