#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#define MAX_QUEUE_SIZE 100
#define INITIAL_PRODUCTION_RATE 1
#define CONSUMER_INTERVAL 10
#define CHECK_INTERVAL 1
#define SLOW_DOWN_FACTOR 1000000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int messageQueue[MAX_QUEUE_SIZE];
int queueSize = 0;
int productionRate = INITIAL_PRODUCTION_RATE;
FILE *fptr;

void *producer(void *arg){
    int currentRate;
    while (1){
        pthread_mutex_lock(&mutex);

        // Produce message

        if (queueSize < MAX_QUEUE_SIZE){
            messageQueue[queueSize++] = 1;
            //fprintf(fptr, "%d\n", queueSize);
        }
        else{
            printf("OVERFLOW, the production rate was: %d", productionRate);
            fprintf(fptr, "%d\n", queueSize);
            fclose(fptr);
            exit(1);
        }

        printf("Produced. Queue size: %d\n", queueSize);

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

        if (queueSize == 0){pthread_cond_wait(&cond, &mutex);}

        // Consume message
        messageQueue[--queueSize] = 0;
        printf("Consumed. Queue size: %d\n", queueSize);
        //fprintf(fptr, "%d\n", queueSize);

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
        if (queueSize < MAX_QUEUE_SIZE / 2){
            productionRate +=  (MAX_QUEUE_SIZE-queueSize)/10;
            //productionRate += 1;
            printf("Increased production rate to %d\n", productionRate);
        }
        else if (queueSize >= (2 * MAX_QUEUE_SIZE / 3)){
            productionRate /= (queueSize);
            //productionRate -= 5;

            if(productionRate<=0) productionRate=1;
            printf("Decreased production rate to %d\n", productionRate);
        }
        printf("Queue length: %d\n", queueSize);
        fprintf(fptr, "%d\n", queueSize);
        fflush(fptr);
        
        pthread_mutex_unlock(&mutex);

        
    }

    pthread_exit(NULL);
}

int main(){
    // Open a file in writing mode
    fptr = fopen("filename.txt", "w");
    
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
