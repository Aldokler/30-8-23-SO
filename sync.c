#include <math.h>
#include <time.h>  
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

pthread_mutex_t mutex;
//int threads = 10;
int n = 0;
pthread_cond_t cond;

void barrier(int threads){
    pthread_mutex_lock(&mutex);
    n++;
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    if (n == threads){
        pthread_cond_broadcast(&cond);
    }
    else{
         pthread_cond_wait(&cond, &mutex);
    }

    pthread_mutex_unlock(&mutex);
}