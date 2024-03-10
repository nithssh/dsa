#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *buffer;
  int size;
  int capacity;
} Heap;

Heap create(int);
int poll(Heap *);
void insert(Heap *, int);
void heapifyDown(Heap *, int);
void heapifyUp(Heap *, int);
void display(Heap *);

int leftChild(int);
int rightChild(int);
int hasRightChild(Heap *, int);
int parent(int);

int main() {
  Heap heap;
  int selection = -1;
  while (selection) {
    printf("0. Exit\n");
    printf("1. Create Heap\n");
    printf("2. Insert\n");
    printf("3. Poll\n");
    printf("> ");
    scanf("%d", &selection);

    switch (selection) {
    case 0:
      printf("Exiting...\n");
      break;
    case 1:
      printf("Capacity > ");
      int cap;
      scanf("%d", &cap);
      heap = create(cap);
      break;
    case 2:
      printf("Value > ");
      int val;
      scanf("%d", &val);
      insert(&heap, val);
      display(&heap);
      break;
    case 3:
      int top = poll(&heap);
      printf("Got: %d\n", top);
      display(&heap);
      break;
    }
  }
}

Heap create(int size) {
  // 1-based buffer
  int actualCapacity = pow(2, ceil(log2(size)));
  Heap heap = {calloc(actualCapacity, sizeof(int)), 0, actualCapacity - 1};
  return heap;
}

int poll(Heap *heap) {
  if (heap->size == 0) {
    return -1;
  }

  int result = heap->buffer[1];
  heap->buffer[1] = heap->buffer[heap->size];
  heap->size--;
  heapifyDown(heap, 1);

  // shrink the buffer if the size is low enough now
  if (heap->size <= heap->capacity / 2) {
    int newCapacity = (heap->capacity / 2) + 1;
    heap->buffer = realloc(heap->buffer, sizeof(int) * newCapacity);
    heap->capacity = newCapacity - 1;
  }

  return result;
}

void insert(Heap *heap, int value) {
  // Grow the buffer if it will overflow
  if (heap->size == heap->capacity) {
    int newCapacity = (heap->capacity + 1) * 2;
    heap->buffer = realloc(heap->buffer, newCapacity * sizeof(int));
    heap->capacity = newCapacity - 1;
  }

  heap->size++;
  heap->buffer[heap->size] = value;
  heapifyUp(heap, heap->size);
}

void display(Heap *heap) {
  int rows = log2(heap->capacity + 1);
  for (int i = 0; i < rows; i++) {
    int width = pow(2, i);
    for (int j = 0; j < width; j++) {
      int linearIdx = (int)pow(2, i) + j;
      if (linearIdx > heap->size) {
        printf("- ");
      } else {
        printf("%d ", heap->buffer[linearIdx]);
      }
    }
    printf("\n");
  }
}

void heapifyUp(Heap *heap, int index) {
  if (index <= 1) {
    return;
  }
  if (heap->buffer[parent(index)] < heap->buffer[index]) {
    int temp = heap->buffer[parent(index)];
    heap->buffer[parent(index)] = heap->buffer[index];
    heap->buffer[index] = temp;
    heapifyUp(heap, parent(index));
  }
}

void heapifyDown(Heap *heap, int index) {
  // Check if we are at leaf
  if (leftChild(index) > heap->size) {
    return;
  }

  int childIdx = leftChild(index);
  // check if right child exists and is bigger
  if (hasRightChild(heap, index) &&
      heap->buffer[rightChild(index)] >= heap->buffer[leftChild(index)]) {
    childIdx = rightChild(index);
  }

  // check if the value needs to be swapped.
  if (heap->buffer[childIdx] <= heap->buffer[index]) {
    return;
  }

  int *childPtr = &heap->buffer[childIdx];
  int temp = *childPtr;
  *childPtr = heap->buffer[index];
  heap->buffer[index] = temp;
  heapifyDown(heap, childIdx);
}

// helper functions
inline int leftChild(int index) { return index * 2; }

inline int rightChild(int index) { return index * 2 + 1; }

inline int parent(int index) { return index / 2; }

inline int hasRightChild(Heap *heap, int index) {
  return rightChild(index) <= heap->capacity;
}
