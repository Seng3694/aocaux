#include "common.h"

// these macros have to be defined
// #define AOC_T
// #define AOC_T_NAME
// #define AOC_T_COMPARE
// uses MinHeap as default. use this for MaxHeap:
// #define AOC_MAX_HEAP

#ifndef AOC_T
#error "AOC_T must be defined"
#endif

#ifndef AOC_T_NAME
#error "AOC_T_NAME must be defined"
#endif

#ifndef AOC_T_COMPARE
#error "AOC_T_COMPARE must be defined"
#endif

#ifndef AOC_SIZE_T
#include <stddef.h>
#define AOC_SIZE_T size_t
#endif

#ifndef AOC_MALLOC
#include <stdlib.h>
#define AOC_MALLOC malloc
#endif

#ifndef AOC_REALLOC
#include <stdlib.h>
#define AOC_REALLOC realloc
#endif

#ifndef AOC_FREE
#include <stdlib.h>
#define AOC_FREE free
#endif

#ifdef AOC_MAX_HEAP
#define HP_TYPE Max
#else
#define HP_TYPE Min
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define HP_NAME COMBINE2(COMBINE2(COMBINE2(Aoc, HP_TYPE), Heap), AOC_T_NAME)
#define HP_IMPL(word) COMBINE2(HP_NAME, word)
#define HP_IMPL_INTERNAL(word) COMBINE2(_, COMBINE2(HP_NAME, word))
#define HP_LINKAGE static inline

#define HP_PARENT(i) (((i)-1) / 2)
#define HP_LEFT_CHILD(i) (2 * (i) + 1)
#define HP_RIGHT_CHILD(i) (2 * (i) + 2)

typedef struct {
  AOC_SIZE_T capacity;
  AOC_SIZE_T count;
  AOC_T *items;
} HP_NAME;

#define HP_CREATE HP_IMPL(Create)
#define HP_DESTROY HP_IMPL(Destroy)
#define HP_PEEK HP_IMPL(Peek)
#define HP_PUSH HP_IMPL(Push)
#define HP_POP HP_IMPL(Pop)

#define HP_SWAP HP_IMPL_INTERNAL(Swap)
#define HP_ENSURE_CAPACITY HP_IMPL_INTERNAL(EnsureCapacity)
#define HP_HEAPIFY_UP HP_IMPL_INTERNAL(HeapifyUp)
#define HP_HEAPIFY_DOWN HP_IMPL_INTERNAL(HeapifyDown)

HP_LINKAGE void HP_CREATE(HP_NAME *const h, const AOC_SIZE_T capacity);
HP_LINKAGE void HP_DESTROY(HP_NAME *const h);
HP_LINKAGE void HP_PUSH(HP_NAME *const h, AOC_T item);
HP_LINKAGE AOC_T HP_POP(HP_NAME *const h);
HP_LINKAGE AOC_T HP_PEEK(const HP_NAME *const h);

HP_LINKAGE void HP_SWAP(AOC_T *const a, AOC_T *const b);
HP_LINKAGE void HP_ENSURE_CAPACITY(HP_NAME *const h, AOC_SIZE_T capacity);
HP_LINKAGE void HP_HEAPIFY_UP(HP_NAME *const h);
HP_LINKAGE void HP_HEAPIFY_DOWN(HP_NAME *const h);

HP_LINKAGE void HP_CREATE(HP_NAME *const h, const AOC_SIZE_T capacity) {
  h->capacity = capacity;
  h->count = 0;
  h->items = (AOC_T *)AOC_MALLOC(sizeof(AOC_T) * capacity);
}

HP_LINKAGE void HP_DESTROY(HP_NAME *const h) {
  AOC_FREE(h->items);
}

HP_LINKAGE void HP_PUSH(HP_NAME *const h, AOC_T item) {
  HP_ENSURE_CAPACITY(h, h->count + 1);
  h->items[h->count++] = item;
  HP_HEAPIFY_UP(h);
}

HP_LINKAGE AOC_T HP_POP(HP_NAME *const h) {
  const AOC_T value = HP_PEEK(h);
  h->count--;
  h->items[0] = h->items[h->count];
  HP_HEAPIFY_DOWN(h);
  return value;
}

HP_LINKAGE AOC_T HP_PEEK(const HP_NAME *const h) {
  return h->items[0];
}

HP_LINKAGE void HP_SWAP(AOC_T *const a, AOC_T *const b) {
  const AOC_T tmp = *a;
  *a = *b;
  *b = tmp;
}

HP_LINKAGE void HP_ENSURE_CAPACITY(HP_NAME *const h, AOC_SIZE_T capacity) {
  if (h->capacity < capacity) {
    h->capacity = capacity * 2;
    h->items = (AOC_T *)AOC_REALLOC(h->items, sizeof(AOC_T) * h->capacity);
  }
}

HP_LINKAGE void HP_HEAPIFY_UP(HP_NAME *const h) {
  int64_t index = (int64_t)h->count - 1;
  for (;;) {
    const int64_t parent = HP_PARENT(index);

#ifdef AOC_MAX_HEAP
    if (parent < 0 || AOC_T_COMPARE(&h->items[parent], &h->items[index]) >= 0)
#else
    if (parent < 0 || AOC_T_COMPARE(&h->items[parent], &h->items[index]) <= 0)
#endif
      break;

    HP_SWAP(&h->items[parent], &h->items[index]);
    index = parent;
  }
}

HP_LINKAGE void HP_HEAPIFY_DOWN(HP_NAME *const h) {
  int64_t index = 0;
  for (;;) {
    const int64_t leftChild = HP_LEFT_CHILD(index);
    if (leftChild > h->count)
      break;

    const int64_t rightChild = HP_RIGHT_CHILD(index);

#ifdef AOC_MAX_HEAP
    const int64_t bsChild =
        rightChild < h->count &&
                AOC_T_COMPARE(&h->items[rightChild], &h->items[leftChild]) > 0
            ? rightChild
            : leftChild;
    if (AOC_T_COMPARE(&h->items[index], &h->items[bsChild]) > 0)
      break;
#else
    const int64_t bsChild =
        rightChild < h->count &&
                AOC_T_COMPARE(&h->items[rightChild], &h->items[leftChild]) < 0
            ? rightChild
            : leftChild;
    if (AOC_T_COMPARE(&h->items[index], &h->items[bsChild]) < 0)
      break;
#endif

    HP_SWAP(&h->items[index], &h->items[bsChild]);
    index = bsChild;
  }
}

#undef COMBINE
#undef COMBINE2

#undef AOC_T
#undef AOC_T_NAME
#undef AOC_T_COMPARE
#undef AOC_MAX_HEAP
#undef AOC_MALLOC
#undef AOC_REALLOC
#undef AOC_FREE
#undef AOC_SIZE_T

#undef HP_TYPE
#undef HP_NAME
#undef HP_IMPL
#undef HP_IMPL_INTERNAL
#undef HP_LINKAGE
#undef HP_PARENT
#undef HP_LEFT_CHILD
#undef HP_RIGHT_CHILD

#undef HP_CREATE
#undef HP_DESTROY
#undef HP_PEEK
#undef HP_PUSH
#undef HP_POP

#undef HP_SWAP
#undef HP_ENSURE_CAPACITY
#undef HP_HEAPIFY_UP
#undef HP_HEAPIFY_DOWN
