#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10
#define NUM_SORT_THREADS 4
#define NUM_MERGE_THREADS 3

int arr[SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8}; // Predefined array

typedef struct SubArray {
    int from_index;
    int to_index;
} parameters;

void merge(int from_index, int to_index) {
    int mid = (from_index + to_index) / 2;
    int left = from_index;
    int right = mid + 1;

    int tempArray[to_index - from_index + 1]; // temp array for merge
    int temp = 0;

    while (left <= mid && right <= to_index) {
        if (arr[left] <= arr[right])
            tempArray[temp++] = arr[left++];
        else
            tempArray[temp++] = arr[right++];
    }

    while (left <= mid)
        tempArray[temp++] = arr[left++];

    while (right <= to_index)
        tempArray[temp++] = arr[right++];

    for (int i = 0; i < (to_index - from_index + 1); i++)
        arr[from_index + i] = tempArray[i];
}

void *mergesort(void *param) {
    parameters *p = (parameters *)param;
    int mid = (p->from_index + p->to_index) / 2;

    parameters a[NUM_SORT_THREADS];
    pthread_t thread[NUM_SORT_THREADS];

    a[0].from_index = p->from_index;
    a[0].to_index = mid;

    a[1].from_index = mid + 1;
    a[1].to_index = p->to_index;

    if (p->from_index >= p->to_index)
        return 0;

    for (int i = 0; i < NUM_SORT_THREADS; i++) {
        if (i < 2) { // first two threads sort first half
            pthread_create(&thread[i], NULL, mergesort, &a[i]);
        } else if (i == 2) { // sorts the second half using remaining threads
            a[i].from_index = mid + 1;
            a[i].to_index = p->to_index;
            pthread_create(&thread[i], NULL, mergesort, &a[i]);
        }
    }

    for (int i = 0; i < NUM_SORT_THREADS; i++)
        pthread_join(thread[i], NULL); // waits for sorting threads to finish

    merge(p->from_index, p->to_index);
    return 0;
}

void *merging(void *param) {
    parameters *p = (parameters *)param;
    merge(p->from_index, p->to_index);
    return 0;
}

int main(int argc, const char *argv[]) {
    parameters i;
    i.from_index = 0;
    i.to_index = SIZE - 1;

    pthread_t thread; // new thread

    pthread_create(&thread, NULL, mergesort, &i);
    pthread_join(thread, NULL);

    parameters mergeArray[NUM_MERGE_THREADS];
    pthread_t mergeThreads[NUM_MERGE_THREADS];

    mergeArray[0].from_index = 0;
    mergeArray[0].to_index = (SIZE / 4) - 1; // merge 1st and 2nd thread

    mergeArray[1].from_index = (SIZE / 4);
    mergeArray[1].to_index = (SIZE / 2) - 1; // merge 3rd and 4th thread

    mergeArray[2].from_index = (SIZE / 2);
    mergeArray[2].to_index = SIZE - 1; // merge two halves


    for (int i = 0; i < NUM_MERGE_THREADS; i++) {
        pthread_create(&mergeThreads[i], NULL, merging, &mergeArray[i]);
    }

    for (int i = 0; i < NUM_MERGE_THREADS; i++) {
        pthread_join(mergeThreads[i], NULL); // Wait for merge threads to finish
    }

    // Print the sorted result
    printf("Sorted list:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
