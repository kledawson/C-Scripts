#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#define numElement 10
#define numReaders 3
#define numWriters 2
#define maxOperations 100

int sharedArray[numElement];
int activeReaders = 0;
int writers_waiting = 0;
int activeWriters = 0;
int totalReadOperations = 0;
int totalWriteOperations = 0;

pthread_mutex_t lock;
pthread_cond_t readerConditions;
pthread_cond_t writerConditions;

//reader threads
void* reader(void* arg) {
    int id = *((int*)arg);
    while (1) {
        pthread_mutex_lock(&lock);

        while (activeWriters > 0 || writers_waiting > 0) {
            pthread_cond_wait(&readerConditions, &lock);
        }
        activeReaders++;
        pthread_mutex_unlock(&lock);

        printf("Reader thread number %d is currently reading the array...", id);
        printf("\n");
        for (int i = 0; i < numElement; i++) {
            printf("%d ", sharedArray[i]);
        }
        printf("\n");
        sleep(rand() % 2);

        pthread_mutex_lock(&lock);
        activeReaders--;
        totalReadOperations++;

        if (activeReaders == 0 && writers_waiting > 0) {
            pthread_cond_signal(&writerConditions);
        }

        if (totalReadOperations + totalWriteOperations >= maxOperations) {
            pthread_mutex_unlock(&lock);
            break;
        }

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

//writer threads
void* writer(void* arg) {
    int id = *((int*)arg);
    while (1) {
        pthread_mutex_lock(&lock);
        writers_waiting++;

        while (activeReaders > 0 || activeWriters > 0) {
            pthread_cond_wait(&writerConditions, &lock);
        }
        writers_waiting--;
        activeWriters++;
        pthread_mutex_unlock(&lock);

        printf("Writer thread number %d is currently writing to the array...", id);
        printf("\n");

        srand(12);
        for (int i = 0; i < numElement; i++) {
            sharedArray[i] = rand() % 100;
        }
        sleep(rand() % 2);

        pthread_mutex_lock(&lock);
        activeWriters--;
        totalWriteOperations++;

        if (writers_waiting > 0) {
            pthread_cond_signal(&writerConditions);
        } else {
            pthread_cond_broadcast(&readerConditions);
        }

        if (totalReadOperations + totalWriteOperations >= maxOperations) {
            pthread_mutex_unlock(&lock);
            break;
        }

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t readers[numReaders];
    pthread_t writers[numWriters];
    int readerIDs[numReaders];
    int writerIDs[numWriters];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&readerConditions, NULL);
    pthread_cond_init(&writerConditions, NULL);

    //creating global array
    for (int i = 0; i < numElement; i++) {
        sharedArray[i] = -1;
    }

    //reader and writer threads
    for (int i = 0; i < numReaders; i++) {
        readerIDs[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &readerIDs[i]);
    }
    for (int i = 0; i < numWriters; i++) {
        writerIDs[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writerIDs[i]);
    }

    //waiting for signal
    for (int i = 0; i < numReaders; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < numWriters; i++) {
        pthread_join(writers[i], NULL);
    }

    //removing threads
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&readerConditions);
    pthread_cond_destroy(&writerConditions);
    printf("All operations completed...terminating program...\n");
    return 0;
}
