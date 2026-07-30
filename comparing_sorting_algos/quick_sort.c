/*
 Name - Abdulla
 Roll Number - CrS2501
 Year - 2025
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int partition(int A[], int p, int r);
void quickSort(int A[], int p, int r);

void createRandomArray(int A[],int n);
void printArray(int A[], int n);

int main() {
    //seeding rand function using time
    srand(time(NULL));

    int n;
    printf("Enter an integer greater than zero : ");
    scanf("%d",&n);
    //Input validity checking
    if (n < 0) {
        printf("Invalid Input\n");
        return -1;
    }


    int *A;
    A = (int*) malloc(n * sizeof(int));
    if( A == NULL) {
        printf("Memory Allocation failed\n");
        return -1;
    }

    // inserting n elements to A randomly using rand function.
    createRandomArray(A, n);
    printf("Generated Array of size %d :\n", n);
    printArray(A, n);

    //quicksort
    quickSort(A, 0, n - 1);
    printf("Sorted Array :\n");
    printArray(A, n);
    
    //freeing allocated memory to prevent memory leaking.
    free(A);

    return 0;
}

int partition(int A[], int p, int r) {
    int temp, i = p - 1, x = A[r];
    for (int j = p; j < r; j++) {
        if (A[j] <= x) {
            i++;
//swapping A[i] & A[j]
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;   
        }
    }
//swapping A[i+1] & A[r]
    temp = A[i+1];
    A[i+1] = A[r];
    A[r] = temp; 
    return i+1; //returns the correct position of x in the sorted array.
}

void quickSort(int A[], int p, int r) {
    if(p < r) {
        int q = partition(A, p, r);
//breaking down the array recursively
        quickSort(A, p, q - 1);
        quickSort(A, q + 1, r);
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



