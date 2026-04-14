#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "buffer.h"

void *producer(void *param) {
    buffer_item item;
    while (1) {
        sleep(rand() % 5 + 1);

        item = rand();

        if (insert_item(item)) {
            fprintf(stderr, "Producer could not insert item\n");
        } else {
            printf("Producer produced %d\n", item);
        }
    }
    return NULL;
}

void *consumer(void *param) {
    buffer_item item;
    while (1) {
        sleep(rand() % 5 + 1);

        if (remove_item(&item)) {
            fprintf(stderr, "Consumer could not remove item\n");
        } else {
            printf("Consumer consumed %d\n", item);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <sleep_time> <num_producers> <num_consumers>\n", argv[0]);
        return 1;
    }

    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    srand(time(NULL)); // seed rng

    initialize();

    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    // producer threads
    for (int i = 0; i < num_producers; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }

    // consumer threads
    for (int i = 0; i < num_consumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    sleep(sleep_time);
    cleanup();

    return 0;
}
