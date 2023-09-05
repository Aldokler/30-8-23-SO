#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include<unistd.h>

#define MAX_SIZE 100  

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


int main(void){
      Semaphore sema;
      pthread_t t1;
      pthread_t t2;
      pthread_t t3;
      
      printf("start\n");
      
      semaphore_init(&sema, 2);
      
      printf("outside%d\n", sema.resources);
      
      void* timewaste(){
      
      	semaphore_wait(&sema);
      
        printf("usando recursos \n");
        sleep(3);
        
        semaphore_signal(&sema);
      }
      
      
      pthread_create(&t1,NULL,&timewaste, NULL);
      pthread_create(&t2,NULL,&timewaste, NULL);
      pthread_create(&t3,NULL,&timewaste, NULL);
      
      pthread_join(t1,NULL);
      pthread_join(t2,NULL);
      pthread_join(t3,NULL);
      
      
      
      

}

