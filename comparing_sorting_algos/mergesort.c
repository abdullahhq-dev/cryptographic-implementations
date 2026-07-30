/*
 Name - Abdulla
 Roll Number - CrS2501
 Year - 2025
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge(int A[], int p, int q, int r);
void mergeSort(int A[], int p, int r);

void createRandomArray(int A[],int n);
void printArray(int A[], int n);

int main() {
    srand(time(NULL));

    int n;
    printf("Enter an integer greater than zero : ");
    scanf("%d",&n);
    //Input validity checking
    if (n < 0) {
        printf("Invalid Input\n");
        return -1;
    }

    // manually allocating memory for an integer array of size n.
    int *A;
    A = (int*) malloc(n * sizeof(int));
    // handling memory allocation failure cases.
    if( A == NULL) {
        printf("Memory Allocation failed\n");
        return -1;
    }

    // inserting n elements to A randomly using rand function.
    createRandomArray(A, n);
    printf("Generated Array of size %d :\n", n);
    printArray(A, n);

    //Mergesorting
    mergeSort(A, 0, n - 1);
    printf("Sorted Array :\n");
    printArray(A, n);
    
    //freeing allocated memory to prevent memory leaking.
    free(A);

    return 0;
}

void merge(int A[], int p, int q, int r) {
    int n1 = q - p + 1, n2 = r - q; 
    // n1 = size of A[p...q],n2 = size of A[q+1..r].
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
    //copying rest of the elements from the subarray which has elements remaining
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
   //copying sorted Subarray from B to the correct part of A 
    for (int i = 0; i < n1 + n2; i++) {
        A[p + i] = B[i];
    } 
}

void mergeSort(int A[], int p, int r) {
    if (p < r) {
        int q = (p + r) / 2; //integer division to get floor p+q/2

        mergeSort(A, p, q);
        mergeSort(A, q + 1, r);
        merge(A, p, q, r);
    }
}

void createRandomArray(int A[],int n) {
    for (int i = 0; i < n; i++) {
        A[i] = 50 - rand() % 100; // randomly generates numbers between -49 abd 50
    } 
}

void printArray(int A[], int n) {
    printf("[ ");
    for (int i = 0; i < n; i++) {
        printf("%d ",A[i]);
    } 
    printf("]\n");
}
    