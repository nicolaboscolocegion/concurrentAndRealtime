#include "queue.h"

void initializeQueue(queue *q, int size) {
    if (size < 0) {
        q = NULL;
        return;
    }
    q->items = (int *)malloc(size * sizeof(int));
    q->maxSize = size;
    q->front = -1;
    q->rear = -1;
}

int isFull(queue *q) {
    if ((q->front == q->rear + 1) ||
        (q->front == 0 && q->rear == q->maxSize - 1))
        return 1;
    return 0;
}

int isEmpty(queue *q) {
    if (q->front == -1) return 1;
    return 0;
}

void enqueue(queue *q, int element) {
    if (q->front == -1) q->front = 0;
    q->rear = (q->rear + 1) % q->maxSize;
    q->items[q->rear] = element;
}

int dequeue(queue *q) {
    int element;

    element = q->items[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % q->maxSize;
    }
    return element;
}

int size(queue *q) {
    if (isEmpty(q)) {
        return 0;
    } else if (q->front <= q->rear) {
        return q->rear - q->front + 1;
    } else {
        return q->maxSize - q->front + q->rear + 1;
    }
}

void freeQueue(queue *q) {
    free(q->items);
    q->items = NULL;
    q->front = -1;
    q->rear = -1;
    q->maxSize = 0;
}
