#include "sync.h"

void merge(int L[], int R[]){
    int n = (sizeof(L) / sizeof(L[0])) + (sizeof(R) / sizeof(R[0])); 
}

void mergeSort(int arr[]){
    int n = (sizeof(arr) / sizeof(arr[0])) - 1;
    if (n != 0){
        int L[n/2], R[n/2], result[n];
        for (int i = 0; i < n/2; i++){
            L[i] = arr[i];
        }
        for (int i = n/2; i < n; i++){
            R[i] = arr[i];
        }

        //hilos

        mergeSort(L);
        mergeSort(R);

        return merge(L, R);
    }
}