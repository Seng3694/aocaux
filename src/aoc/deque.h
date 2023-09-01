#include "common.h"
#include "mem.h"

#ifndef AOC_T
#error "AOC_T must be defined"
#endif

#ifndef AOC_T_NAME
#error "AOC_T_NAME must be defined"
#endif

#ifdef AOC_BASE2_CAPACITY
#define DEQUE_MOD(a, b) ((a) & ((b)-1))
#else
#define DEQUE_MOD(a, b) ((a) % (b))
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define DEQUE_NAME COMBINE2(AocDeque, AOC_T_NAME)
#define DEQUE_IMPL(word) COMBINE2(DEQUE_NAME, word)
#define DEQUE_LINKAGE static inline

typedef struct {
  AOC_SIZE_T capacity;
  AOC_SIZE_T length;
  AOC_SIZE_T head;
  AOC_SIZE_T tail;
  AOC_T *items;
} DEQUE_NAME;

#define DEQUE_CREATE DEQUE_IMPL(Create)
#define DEQUE_DESTROY DEQUE_IMPL(Destroy)
#define DEQUE_COPY DEQUE_IMPL(Copy)
#define DEQUE_DUPE DEQUE_IMPL(Duplicate)
#define DEQUE_PUSH_FRONT DEQUE_IMPL(PushFront)
#define DEQUE_PUSH_BACK DEQUE_IMPL(PushBack)
#define DEQUE_POP_FRONT DEQUE_IMPL(PopFront)
#define DEQUE_POP_BACK DEQUE_IMPL(PopBack)
#define DEQUE_PEEK_FRONT DEQUE_IMPL(PeekFront)
#define DEQUE_PEEK_BACK DEQUE_IMPL(PeekBack)
#define DEQUE_ENSURE_CAPACITY DEQUE_IMPL(EnsureCapacity)
#define DEQUE_RESIZE DEQUE_IMPL(Resize)
#define DEQUE_CLEAR DEQUE_IMPL(Clear)

DEQUE_LINKAGE void DEQUE_CREATE(DEQUE_NAME *const deque,
                                const AOC_SIZE_T capacity);
DEQUE_LINKAGE void DEQUE_DESTROY(DEQUE_NAME *const deque);
DEQUE_LINKAGE void DEQUE_COPY(DEQUE_NAME *const dest,
                              const DEQUE_NAME *const src);
DEQUE_LINKAGE void DEQUE_DUPE(DEQUE_NAME *const dest,
                              const DEQUE_NAME *const src);
DEQUE_LINKAGE void DEQUE_PUSH_FRONT(DEQUE_NAME *const deque, AOC_T item);
DEQUE_LINKAGE void DEQUE_PUSH_BACK(DEQUE_NAME *const deque, AOC_T item);
DEQUE_LINKAGE void DEQUE_POP_FRONT(DEQUE_NAME *const deque);
DEQUE_LINKAGE void DEQUE_POP_BACK(DEQUE_NAME *const deque);
DEQUE_LINKAGE AOC_T *DEQUE_PEEK_FRONT(DEQUE_NAME *const deque);
DEQUE_LINKAGE AOC_T *DEQUE_PEEK_BACK(DEQUE_NAME *const deque);
DEQUE_LINKAGE void DEQUE_ENSURE_CAPACITY(DEQUE_NAME *const deque,
                                         const AOC_SIZE_T capacity);
DEQUE_LINKAGE void DEQUE_RESIZE(DEQUE_NAME *const deque,
                                const AOC_SIZE_T capacity);
DEQUE_LINKAGE void DEQUE_CLEAR(DEQUE_NAME *const deque);

DEQUE_LINKAGE void DEQUE_CREATE(DEQUE_NAME *const deque,
                                const AOC_SIZE_T capacity) {
  deque->capacity = capacity;
  deque->length = 0;
  deque->head = 0;
  deque->tail = 0;
  deque->items = (AOC_T *)AOC_MALLOC(sizeof(AOC_T) * capacity);
}

DEQUE_LINKAGE void DEQUE_DESTROY(DEQUE_NAME *const deque) {
  AOC_FREE(deque->items);
  deque->items = NULL;
}

DEQUE_LINKAGE void DEQUE_COPY(DEQUE_NAME *const dest,
                              const DEQUE_NAME *const src) {
  DEQUE_ENSURE_CAPACITY(dest, src->capacity);
  AocMemCopy(dest->items, src->items, sizeof(AOC_T) * src->length);
  dest->length = src->length;
  dest->head = src->head;
  dest->tail = src->tail;
}

DEQUE_LINKAGE void DEQUE_DUPE(DEQUE_NAME *const dest,
                              const DEQUE_NAME *const src) {
  DEQUE_CREATE(dest, src->capacity);
  AocMemCopy(dest->items, src->items, sizeof(AOC_T) * src->length);
  dest->length = src->length;
  dest->head = src->head;
  dest->tail = src->tail;
}

DEQUE_LINKAGE void DEQUE_PUSH_BACK(DEQUE_NAME *const deque, AOC_T item) {
  DEQUE_ENSURE_CAPACITY(deque, deque->length + 1);
  deque->items[deque->tail] = item;
  deque->tail = DEQUE_MOD((deque->tail + 1), deque->capacity);
  deque->length++;
}

DEQUE_LINKAGE void DEQUE_PUSH_FRONT(DEQUE_NAME *const deque, AOC_T item) {
  DEQUE_ENSURE_CAPACITY(deque, deque->length + 1);

  if (deque->head == 0)
    deque->head = deque->capacity - 1;
  else
    deque->head--;

  deque->items[deque->head] = item;
  deque->length++;
}

DEQUE_LINKAGE void DEQUE_POP_BACK(DEQUE_NAME *const deque) {
  AOC_ASSERT(deque->length > 0);

  if (deque->tail == 0)
    deque->tail = deque->capacity - 1;
  else
    deque->tail--;

  deque->length--;
}

DEQUE_LINKAGE void DEQUE_POP_FRONT(DEQUE_NAME *const deque) {
  AOC_ASSERT(deque->length > 0);
  deque->head = DEQUE_MOD((deque->head + 1), deque->capacity);
  deque->length--;
}

DEQUE_LINKAGE AOC_T *DEQUE_PEEK_BACK(DEQUE_NAME *const deque) {
  AOC_ASSERT(deque->length > 0);
  return &deque->items[deque->tail - 1];
}

DEQUE_LINKAGE AOC_T *DEQUE_PEEK_FRONT(DEQUE_NAME *const deque) {
  AOC_ASSERT(deque->length > 0);
  return &deque->items[deque->head];
}

DEQUE_LINKAGE void DEQUE_ENSURE_CAPACITY(DEQUE_NAME *const deque,
                                         const AOC_SIZE_T capacity) {
  if (deque->capacity < capacity) {
    DEQUE_RESIZE(deque, deque->capacity * 2);
  }
}
DEQUE_LINKAGE void DEQUE_RESIZE(DEQUE_NAME *const deque,
                                const AOC_SIZE_T capacity) {
  if (capacity != deque->capacity) {
    if (deque->head >= deque->tail) {
      // example:
      // old array (size 5): [4] [5] [1] [2] [3]   (head 1   tail 5)
      // new array (size 8): [1] [2] [3] [4] [5] [-] [-] [-]
      AOC_T *newData = (AOC_T *)AOC_MALLOC(capacity * sizeof(AOC_T));
      // copy [1] [2] [3]
      AocMemCopy(newData, deque->items + deque->head,
                 (deque->capacity - deque->head) * sizeof(AOC_T));
      // copy [4] [5]
      AocMemCopy(newData + deque->capacity - deque->head, deque->items,
                 deque->tail * sizeof(AOC_T));
      deque->tail = deque->capacity - deque->head + deque->tail;
      deque->head = 0;
      AOC_FREE(deque->items);
      deque->items = newData;
    } else {
      AOC_T *newData = (AOC_T *)AOC_MALLOC(capacity * sizeof(AOC_T));
      AocMemCopy(newData, deque->items, deque->capacity * sizeof(AOC_T));
      AOC_FREE(deque->items);
      deque->items = newData;
    }
  }
  deque->capacity = capacity;
}

DEQUE_LINKAGE void DEQUE_CLEAR(DEQUE_NAME *const deque) {
  deque->length = 0;
  deque->head = 0;
  deque->tail = 0;
}

#undef COMBINE
#undef COMBINE2
#undef AOC_T
#undef AOC_T_NAME
#undef DEQUE_NAME
#undef DEQUE_IMPL
#undef DEQUE_LINKAGE
#undef DEQUE_CREATE
#undef DEQUE_DESTROY
#undef DEQUE_COPY
#undef DEQUE_DUPE
#undef DEQUE_PUSH_FRONT
#undef DEQUE_PUSH_BACK
#undef DEQUE_POP_FRONT
#undef DEQUE_POP_BACK
#undef DEQUE_PEEK_FRONT
#undef DEQUE_PEEK_BACK
#undef DEQUE_ENSURE_CAPACITY
#undef DEQUE_RESIZE
#undef DEQUE_CLEAR
