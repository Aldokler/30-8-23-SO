#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

pthread_mutex_t mutex;
pthread_cond_t cond;


void monitor(bool c,int threads){

    pthread_mutex_lock(&mutex);
    while (c)
    {
        pthread_cond_wait(&mutex, &cond);
    }
    pthread_mutex_unlock(&mutex);

}