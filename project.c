#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "queue.h"

#define MAX_QUEUE_SIZE 100
#define INITIAL_PRODUCTION_RATE 1
#define CONSUMER_INTERVAL 10
#define CHECK_INTERVAL 2
#define SLOW_DOWN_FACTOR 100000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

queue messageQueue;
int productionRate = INITIAL_PRODUCTION_RATE;
FILE *fptr;

void *producer(void *arg){
    int currentRate;
    while (1){
        pthread_mutex_lock(&mutex);

        // Produce message

        if (!isFull(&messageQueue)){
            enqueue(&messageQueue, rand());
            //fprintf(fptr, "%d\n", size(&messageQueue));
        }
        else{
            printf("OVERFLOW, the production rate was: %d", productionRate);
            fprintf(fptr, "%d\n", size(&messageQueue));
            fclose(fptr);
            freeQueue(&messageQueue);
            exit(1);
        }

        printf("Produced. Queue size: %d\n", size(&messageQueue));

        currentRate = productionRate;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);


        usleep(SLOW_DOWN_FACTOR / (currentRate));
    }

    pthread_exit(NULL);
}

void *consumer(void *arg){
    while (1){
        pthread_mutex_lock(&mutex);

        if (isEmpty(&messageQueue)) 
            pthread_cond_wait(&cond, &mutex);

        // Consume message
        dequeue(&messageQueue);
        printf("Consumed. Queue size: %d\n", size(&messageQueue));
        //fprintf(fptr, "%d\n", size(&messageQueue));

        pthread_mutex_unlock(&mutex);
        // Simulate consumed message usage
        usleep(SLOW_DOWN_FACTOR /CONSUMER_INTERVAL);
    }

    pthread_exit(NULL);
}

void *lengthChecker(void *arg){
    while (1){
        usleep(SLOW_DOWN_FACTOR * CHECK_INTERVAL);

        pthread_mutex_lock(&mutex);
        // Adjust production rate
        if (size(&messageQueue) < MAX_QUEUE_SIZE / 2){
            productionRate +=  (MAX_QUEUE_SIZE-size(&messageQueue))/10;
            //productionRate += 1;
            printf("Increased production rate to %d\n", productionRate);
        }
        else if (size(&messageQueue) >= (2 * MAX_QUEUE_SIZE / 3)){
            productionRate /= (size(&messageQueue));
            //productionRate -= 5;

            if(productionRate<=0) productionRate=1;
            printf("Decreased production rate to %d\n", productionRate);
        }
        printf("Queue length: %d\n", size(&messageQueue));
        fprintf(fptr, "%d\n", size(&messageQueue));
        fflush(fptr);
        
        pthread_mutex_unlock(&mutex);

        
    }

    pthread_exit(NULL);
}

int main(){

    initializeQueue(&messageQueue, MAX_QUEUE_SIZE);

    //log file
    fptr = fopen("memUsage.log", "w");
    
    fprintf(fptr, "%d\n", MAX_QUEUE_SIZE);

    pthread_t producerThread, consumerThread, checkerThread;

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);
    pthread_create(&checkerThread, NULL, lengthChecker, NULL);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);
    pthread_join(checkerThread, NULL);

    return 0;
}
