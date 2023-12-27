#pragma once

#include <stdio.h>
#include <stdlib.h>

// simple circular queue implementation

typedef struct {
    int *items;
    int maxSize;
    int front, rear;
} queue;

// inizialize the que given a size, if size<0 the pointer will be NULL
void initializeQueue(queue *q, int size);
// return true/false it's full
int isFull(queue *q);
// return true/false if it's empty
int isEmpty(queue *q);
// insert a element
void enqueue(queue *q, int element);
// remove a elemtent
int dequeue(queue *q);
// returns current number of elements
int size(queue *q);
// free the memory allocated
void freeQueue(queue *q);
