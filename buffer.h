#ifndef BUFFER_H
#define BUFFER_H

#include <semaphore.h>
#include <pthread.h>

typedef int buffer_item;

#define BUFFER_SIZE 5

extern buffer_item buffer[BUFFER_SIZE]; // buffer

extern int in; // next empty slot
extern int out; // next full slot

// semaphores
extern sem_t empty; // empty slots
extern sem_t full; // full slots
extern sem_t mutex; // mutual exclusion

void initialize();
void cleanup();
int insert_item(buffer_item item);
int remove_item(buffer_item *item);

#endif
