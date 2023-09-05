#include <math.h>
#include <time.h>  
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<unistd.h>

pthread_mutex_t mutex;



//int threads = 10;
int n = 0;
pthread_cond_t cond;
#define MAX_SIZE 100  
//semaforoooo
typedef struct{
        pthread_t queue[MAX_SIZE];  
        int front;  
        int rear;  
  

	int resources;
	int maxResources;
	
	pthread_mutex_t semMutex;
	pthread_cond_t condRes;
	
}Semaphore;

void semaphore_init (Semaphore *sem, int maxR);
void semaphore_wait(Semaphore *sem);
void semaphore_signal(Semaphore* sem);

void semaphore_init (Semaphore *sem, int maxR){
	printf("%d\n", maxR);
	sem->front = -1;  
        sem->rear = -1;  
        
	sem->resources = maxR;
	sem->maxResources = maxR;
	
	pthread_mutex_init(&sem->semMutex, NULL);
	pthread_cond_init (&sem->condRes, NULL);
	
	printf("Init exitoso %d\n", sem->resources);
}

void semaphore_wait(Semaphore *sem){
	printf("En wait, recursos: %d\n", sem->resources);
	if (sem->resources > 0)  {
		pthread_mutex_lock(&sem->semMutex);
		--sem->resources;
		pthread_mutex_unlock(&sem->semMutex);
		printf("agarré campo, quedan %d\n", sem->resources);
		
	}else{
		
		if (sem->front == -1 || sem->front > sem->rear) { 
		  
                    if (sem->rear == MAX_SIZE - 1) {  
                        printf("Queue is full");  
                    }  else {
                    
                      if (sem->front == -1) {  
                          sem->front = 0;  
                      }
                      pthread_mutex_lock(&sem->semMutex);
                      sem->rear++;  
                      sem->queue[sem->rear] = pthread_self();
                      pthread_mutex_unlock(&sem->semMutex);
                    }
                    
		  while (sem->resources <= 0){
		      printf("esperando lugar\n");
		      pthread_cond_wait(&sem->condRes, &sem->semMutex);
		      printf("alguien se desocupó\n");
		      
		      printf("%B\n",pthread_equal(sem->queue[sem->front], pthread_self()));
		      
		      if ( !pthread_equal(sem->queue[sem->front], pthread_self()) ){
		      	
				printf("no me toca\n");
				pthread_cond_wait(&sem->condRes, &sem->semMutex);
			}	
		      
		}
		
		pthread_mutex_unlock(&sem->semMutex);
		printf("ya me toca\n");
		if (sem->front == -1 || (sem->front > sem->rear)) {  
			printf("Queue is empty\n");    
		} else{
			printf("Queue not empty\n");   
			pthread_mutex_lock(&sem->semMutex);
			printf("inside mutex\n"); 
			sem->front++;
			--sem->resources;
			pthread_mutex_unlock(&sem->semMutex);
			printf("mutex not the issue\n"); 
		}
		printf("saliendo\n");
                    
                    
		  
		}
	}
	
}

void semaphore_signal(Semaphore* sem){

	        pthread_mutex_lock(&sem->semMutex);
		++sem->resources;
		printf("terminé, quedan %d\n", sem->resources);
		pthread_mutex_unlock(&sem->semMutex);
		printf("avisando\n");
		pthread_cond_signal(&sem->condRes);
}
//barreraaaaa
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
//monitorrrrr
void monitor(bool c){

    pthread_mutex_lock(&mutex);
    while (c)
    {
        pthread_cond_wait(&mutex, &cond);
    }
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

}