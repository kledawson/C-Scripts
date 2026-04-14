#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void initialize() {
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
}

void cleanup() {
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
}

int insert_item(buffer_item item) {
    sem_wait(&empty);
    sem_wait(&mutex);

    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;

    sem_post(&mutex);
    sem_post(&full);

    return 0;
}

int remove_item(buffer_item *item) {
    sem_wait(&full);
    sem_wait(&mutex);

    *item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;

    sem_post(&mutex);
    sem_post(&empty);

    return 0;
}
