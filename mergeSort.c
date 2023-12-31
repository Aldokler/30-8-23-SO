// Merge sort in C
// Editado de https://www.programiz.com/dsa/merge-sort
// Referencia:
// Programiz. Merge Sort Algorithm. Recuperado el 30 de Agosto de 2023 de https://www.programiz.com/dsa/merge-sort

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "sync.c"

int elementos = 11;
int arr[11];
pthread_t hilos[10];
pthread_mutex_t lock;
int h = 0;
int Gp, Gq, Gr;

// Merge two subarrays L and M into arr
void *merge(void *arg){
    // Create L ← A[p..q] and M ← A[q+1..r]
    //pthread_mutex_lock(&lock);
    int n1 = Gq - Gp + 1;
    int n2 = Gr - Gq;
    // Maintain current index of sub-arrays and main array
    int i, j, k;
    i = 0;
    j = 0;
    k = Gp;

    int L[n1], M[n2];

    for (int i = 0; i < n1; i++){
        L[i] = arr[Gp + i];
    }
    for (int j = 0; j < n2; j++){
        M[j] = arr[Gq + 1 + j];
    }

    // Until we reach either end of either L or M, pick larger among
    // elements L and M and place them in the correct position at A[p..r]
    while (i < n1 && j < n2)
    {
        if (L[i] <= M[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = M[j];
            j++;
        }
        k++;
    }

    // When we run out of elements in either L or M,
    // pick up the remaining elements and put in A[p..r]
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = M[j];
        j++;
        k++;
    }
    //pthread_mutex_unlock(&lock);

    barrier(elementos);
}

// Divide the array into two subarrays, sort them and merge them
void mergeSort(int l, int r){
    if (l < r){

        // m is the point where the array is divided into two subarrays
        int m = l + (r - l) / 2;

        mergeSort(l, m);
        mergeSort(m+1, r);

        // Merge the sorted subarrays
        //pthread_mutex_lock(&lock);
        Gp = l;
        Gq = m;
        Gr = r;
        
        pthread_create(&hilos[h], NULL, &merge, NULL);
        h++;
        //pthread_mutex_unlock(&lock);
        //merge(l, m, r);
    }
}

// Print the array
void printArray(int size){
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Driver program
int main(){
    time_t t;

    /* Intializes random number generator */
    srand((unsigned)time(&t));
    for (int i = 0; i < elementos; i++){
        arr[i] = rand() % 1000;
    }
    int size = elementos;

    mergeSort(0, size-1);

    /*
    for (int i = 0; i <= elementos; i++){
        pthread_join(hilos[i], NULL);
    }
    */
    pthread_mutex_destroy(&lock);

    printf("Sorted array: \n");
    printArray(size);
}