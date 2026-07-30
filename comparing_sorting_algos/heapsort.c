#include <stdio.h>
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
int main(){
    int n; scanf("%d",&n);
    int A[n]; for(int i=0;i<n;i++) scanf("%d",&A[i]);
    heapSort(A,n);
    for(int i=0;i<n;i++) printf("%d ",A[i]);
}
