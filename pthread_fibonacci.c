#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// hold arguments
typedef struct {
    int *fibSequence;
    int count;
} FibArgs;

// fibonacci function
void* generateFib(void *args) {
    FibArgs *fibArgsPtr = (FibArgs*)args;
    int *fibSequence = fibArgsPtr->fibSequence;
    int n = fibArgsPtr->count;

    if (n >= 1) fibSequence[0] = 0;
    if (n >= 2) fibSequence[1] = 1;

    for (int i = 2; i < n; i++) {
        fibSequence[i] = fibSequence[i - 1] + fibSequence[i - 2];
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    // error handlers
    if (argc != 2) {
        printf("Please enter number of Fibonacci numbers to generate\n");
        return -1;
    }
    int num = atoi(argv[1]);
    if (num <= 0) {
        printf("Please enter a positive integer\n");
        return -1;
    }
    int *fibSequence = (int*)malloc(num * sizeof(int));
    if (fibSequence == NULL) {
        printf("Allocating memory failed\n");
        return -1;
    }

    FibArgs fibArgs;
    fibArgs.fibSequence = fibSequence;
    fibArgs.count = num;

    // fibonacci thread
    pthread_t fibThread;
    if (pthread_create(&fibThread, NULL, generateFib, &fibArgs) != 0) {
        printf("Creating thread failed\n");
        free(fibSequence);
        return -1;
    }
    pthread_join(fibThread, NULL);

    printf("The fibonacci sequence is: \n");
    for (int i = 0; i < num; i++) {
        printf("%d ", fibSequence[i]);
    }

    free(fibSequence);

    return 0;
}
