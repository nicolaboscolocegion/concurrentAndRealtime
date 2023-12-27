#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int *items;
    int maxSize;
    int front, rear;
}queue;

void initializeQueue(queue *q, int size);
int isFull(queue *q);
int isEmpty(queue *q);
void enqueue(queue *q, int element);
int dequeue(queue *q);
int size(queue *q);
void freeQueue(queue *q);

