// @filename: buffer.c
// @author: Alex Banning
// @assignment: Project 5 - Buffer | Consumer and Producer Threads
// @date: November 8th, 2019
// @description: Implementation file for buffer with consumer and producer threads.
//               main function will sleep for a time (given by args in console) then
//               terminate the process.

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TRUE 1
#define BUFFER_SIZE 5

// Global variables, semaphores, and mutex declaration
buffer_item buffer[BUFFER_SIZE];                /* buffer of buffer_item items of size BUFFER_SIZE */
sem_t empty;                                    /* empty semaphore */
sem_t full;                                     /* full semaphore */
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;  /* mutex */
int shouldRun = 1;                              /* shouldRun flag */
int insertPointer = 0, removePointer = 0;       /* insert and remove pointers (indeces for array) */

// @description: main function executes thread creation, semaphore initialization, and thread joining
// @pre: argc = 4, argv => {"./buffer", <sleepTime>, <# of producer threads>, <# of consumer threads>}
// @post: creates the desired number of threads for each sub-process, sleeps for sleepTime
// @usage: ./buffer 5 1 1
int main(int argc, char *argv[])
{
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

    // Check for correct usage
	if(argc != 4)
	{
		fprintf(stderr, "Usage: <sleep time> <# of producer threads> <# of consumer threads>\n");
		return -1;
	}

    // Verify shouldRun flag is set after input
    shouldRun = 1;

	// Call atoi to get arguments
    sleepTime = atoi(argv[1]);
    producerThreads = atoi(argv[2]);
    consumerThreads = atoi(argv[3]);

    // Print input for verification
    printNewLines(1);
    printf("User input:\n");
    printf("-----------------------------------------------------\n");
    printf("Sleep time is: %d\n", sleepTime);
    printf("# of producer threads: %d\n", producerThreads);
    printf("# of consumer threads: %d\n", consumerThreads);
    printf("-----------------------------------------------------\n");

    // Initialize Semaphores
    printf("Initializing Semaphores...\n");
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

	// Create the producer and consumer threads
    printf("Creating threads...\n");
    pthread_t producers[producerThreads];
    pthread_t consumers[consumerThreads];
    for (i = 0; i < producerThreads; i++) {
        pthread_create(&producers[i], 0, producer, (void *) &i);
    }
	for (j = 0; j < consumerThreads; j++) {
        pthread_create(&consumers[j], 0, consumer, (void *) &j);
	}

	// Sleep for user specified time
    printf("Sleeping for %d seconds...\n", sleepTime);
    printNewLines(1);
	sleep(sleepTime);
    printNewLines(1);
    printf("Done sleeping.\n");
    printNewLines(1);

    // Wait for unfinished threads and exit
    printf("Waiting for child threads to terminate.\n");
    shouldRun = 0;
    for (i = 0; i < producerThreads; i++) {
        pthread_join(producers[i], NULL);
    }
	for (j = 0; j < consumerThreads; j++) {
        pthread_join(consumers[j], NULL);
	}
    printf("Child threads joined.\n");

    printNewLines(1);
    
    // Destroy semaphores
    printf("Destroying semaphores...\n");
    sem_destroy(&empty);
    sem_destroy(&full);
    printf("Semaphores Destoryed.\n");

    // Destroy mutex
    printf("Destroying mutex...\n");
    pthread_mutex_destroy(&m);
    printf("Mutex Destroyed.\n");

    printNewLines(1);
    printf("Goodbye.\n");

    exit(0);
}

// @description: producer thread. Adds items to buffer.
// @pre: None
// @post: After sleeping a random number of seconds, will add an item to the buffer
//        as long as the buffer is not full
// @usage: pthread_create(&producers[i], 0, producer, (void *) &i);
void *producer(void *param)
{
    int sleepyTime = (rand() % 4) + 1;
	buffer_item item;
	while(shouldRun) {
        sleep(sleepyTime);
        item = rand();
        if (!insert_item(item)) {
            printf("Producer Produced: %d\n", item);
        }
        else {
            printf("Error producing item! --> %d\n", item);
        }   
	}

    pthread_exit(NULL);
}

// @description: consumer thread. Removes items from the buffer
// @pre: None
// @post: After sleeping a random number of seconds, will remove an item from the buffer
//        as long as the buffer is not empty
// @usage: pthread_create(&consumers[j], 0, consumer, (void *) &j);
void *consumer(void *param)
{
    int sleepyTime = (rand() % 4) + 1;
    buffer_item item;
	while(shouldRun) {
        sleep(sleepyTime);
        if (!remove_item(&item)) {
            printf("Consumer Consumed: %d\n", item);
        }
        else {
            printf("Error consuming an item!\n");
        }
	}

    pthread_exit(NULL);
}

// @description: Inserts an item in the buffer
// @pre: item contains a buffer item to be added
// @post: adds item to the buffer
// @usage: if(!insert_item(item)) {...;}
int insert_item(buffer_item item)
{
    /* Acquire Empty Semaphore */
	sem_wait(&empty);

	while (TRUE) { // wait for lock to be available
        if (!pthread_mutex_trylock(&m))
            break;
    }
	/* Acquire mutex lock to protect buffer */
	buffer[insertPointer] = item;

    if (++insertPointer == BUFFER_SIZE) {
        insertPointer = 0;
    }

	/* Release mutex lock and full semaphore */
	pthread_mutex_unlock(&m);
    sem_post(&full);

	return 0; // upon success
}

// @description: Removes an item from the buffer
// @pre: None
// @post: returns the item removed from the buffer to item
// @usage: if(!remove_item(&item)) {...;}
int remove_item(buffer_item *item)
{
	/* Acquire Full Semaphore */
    sem_wait(&full);

    while (TRUE) { // wait for lock to be available
        if (!pthread_mutex_trylock(&m))
            break;
    }
 
    *item = buffer[removePointer];
    if (++removePointer == BUFFER_SIZE) {
        removePointer = 0;
    }

	/* Release mutex lock and empty semaphore */
	pthread_mutex_unlock(&m);
    sem_post(&empty);

	return 0; // upon success
}

// @description: prints a desired number of new lines
// @pre: desiredNumber contains the desired number of new lines to be printed
// @post: prints the desiredNumber of new lines on the screen
// @usage: printNewLines(2);
void printNewLines(int desiredNumber) {
    for (int i = 0; i < desiredNumber; i++) {
        printf("\n");
    }
}
