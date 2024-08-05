#include <stdlib.h>
#include <stdio.h>

#ifdef Q_VAL

// An array based queue that has constant time dequeueing
struct ringbuf_queue {
    Q_VAL *arr;
    int size;
    int cap;
    int head;
};

struct ringbuf_queue q_new();
void enqueue(ringbuf_queue *q, Q_VAL val);
Q_VAL dequeue(ringbuf_queue *q);

// returns new rbqueue with capacity 10
struct ringbuf_queue q_new() {
    return {
        .arr = (Q_VAL *)malloc(sizeof(Q_VAL) * 10),
        .size = 0,
        .cap = 10,
        .head = 0
    };
}

void enqueue(ringbuf_queue *q, Q_VAL val) {
    int tarIdx = (q->head + q->size) % q->cap;

    // double the value if at limit. No alloc err checks
    if (q->head == tarIdx) {
        int oldCap = q->cap;
        q->cap = q->cap * 2;
        q->arr = (Q_VAL *)realloc(q->arr, sizeof(Q_VAL) * q->cap);

        // unroll the ring to make it contiguous
        int lIdx = 0;
        int rIdx = oldCap;
        while (lIdx < q->head) {
            q->arr[rIdx++] = q->arr[lIdx++];
        }

        tarIdx = oldCap;
    }

    q->arr[tarIdx] = val;
    q->size++;
}

Q_VAL dequeue(ringbuf_queue *q) {
    if (q->size <= 0) {
        printf("Queue underflow\n");
        abort();
    }
    q->size--;
    return q->arr[q->head++];
}

#else
// This of course is a very simple work around to writing somewhat generic code.
// This impl forces you to use vecs of one type only throughout the project
#error You must define Q_VAL with the type to be used for your vector
#endif
