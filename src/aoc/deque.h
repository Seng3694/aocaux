#include "common.h"
#include "mem.h"

#ifndef AOC_T
#error "AOC_T must be defined"
#endif

#ifndef AOC_T_NAME
#define AOC_T_NAME AOC_T
#endif

#ifdef AOC_BASE2_CAPACITY
#define DEQUE_MOD(a, b) ((a) & ((b)-1))
#else
#define DEQUE_MOD(a, b) ((a) % (b))
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define DEQUE_NAME COMBINE2(aoc_deque_, AOC_T_NAME)
#define DEQUE_IMPL(word) COMBINE2(DEQUE_NAME, COMBINE2(_, word))
#define DEQUE_LINKAGE static inline

typedef struct {
  AOC_SIZE_T capacity;
  AOC_SIZE_T length;
  AOC_SIZE_T head;
  AOC_SIZE_T tail;
  AOC_T *items;
} DEQUE_NAME;

#define DEQUE_CREATE DEQUE_IMPL(create)
#define DEQUE_DESTROY DEQUE_IMPL(destroy)
#define DEQUE_COPY DEQUE_IMPL(copy)
#define DEQUE_DUPE DEQUE_IMPL(duplicate)
#define DEQUE_PUSH_FRONT DEQUE_IMPL(push_front)
#define DEQUE_PUSH_BACK DEQUE_IMPL(push_back)
#define DEQUE_POP_FRONT DEQUE_IMPL(pop_front)
#define DEQUE_POP_BACK DEQUE_IMPL(pop_back)
#define DEQUE_PEEK_FRONT DEQUE_IMPL(peek_front)
#define DEQUE_PEEK_BACK DEQUE_IMPL(peek_back)
#define DEQUE_ENSURE_CAPACITY DEQUE_IMPL(ensure_capacity)
#define DEQUE_RESIZE DEQUE_IMPL(resize)
#define DEQUE_CLEAR DEQUE_IMPL(clear)

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
  deque->items = (AOC_T *)aoc_alloc(sizeof(AOC_T) * capacity);
}

DEQUE_LINKAGE void DEQUE_DESTROY(DEQUE_NAME *const deque) {
  aoc_free(deque->items);
  deque->items = NULL;
}

DEQUE_LINKAGE void DEQUE_COPY(DEQUE_NAME *const dest,
                              const DEQUE_NAME *const src) {
  DEQUE_ENSURE_CAPACITY(dest, src->capacity);
  aoc_mem_copy(dest->items, src->items, sizeof(AOC_T) * src->capacity);
  dest->length = src->length;
  dest->head = src->head;
  dest->tail = src->tail;
}

DEQUE_LINKAGE void DEQUE_DUPE(DEQUE_NAME *const dest,
                              const DEQUE_NAME *const src) {
  DEQUE_CREATE(dest, src->capacity);
  aoc_mem_copy(dest->items, src->items, sizeof(AOC_T) * src->capacity);
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
      AOC_T *newData = (AOC_T *)aoc_alloc(capacity * sizeof(AOC_T));
      // copy [1] [2] [3]
      aoc_mem_copy(newData, deque->items + deque->head,
                   (deque->capacity - deque->head) * sizeof(AOC_T));
      // copy [4] [5]
      aoc_mem_copy(newData + deque->capacity - deque->head, deque->items,
                   deque->tail * sizeof(AOC_T));
      deque->tail = deque->capacity - deque->head + deque->tail;
      deque->head = 0;
      aoc_free(deque->items);
      deque->items = newData;
    } else {
      AOC_T *newData = (AOC_T *)aoc_alloc(capacity * sizeof(AOC_T));
      aoc_mem_copy(newData, deque->items, deque->capacity * sizeof(AOC_T));
      aoc_free(deque->items);
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

#define DEQUE_ITER_NAME COMBINE2(aoc_deque_iter_, AOC_T_NAME)
#define DEQUE_ITER_IMPL(word) COMBINE2(DEQUE_ITER_NAME, COMBINE2(_, word))

typedef struct {
  DEQUE_NAME *deque;
  AOC_SIZE_T current;
} DEQUE_ITER_NAME;

#define DEQUE_ITER_INIT DEQUE_ITER_IMPL(init)
#define DEQUE_ITERATE DEQUE_IMPL(iterate)
#define DEQUE_ITERATE_REV DEQUE_IMPL(iterate_rev)

DEQUE_LINKAGE void DEQUE_ITER_INIT(DEQUE_ITER_NAME *const iter,
                                   DEQUE_NAME *const deque);
DEQUE_LINKAGE bool DEQUE_ITERATE(DEQUE_ITER_NAME *const iter, AOC_T *const out);
DEQUE_LINKAGE bool DEQUE_ITERATE_REV(DEQUE_ITER_NAME *const iter,
                                     AOC_T *const out);

DEQUE_LINKAGE void DEQUE_ITER_INIT(DEQUE_ITER_NAME *const iter,
                                   DEQUE_NAME *const deque) {
  iter->deque = deque;
  iter->current = 0;
}

DEQUE_LINKAGE bool DEQUE_ITERATE(DEQUE_ITER_NAME *const iter,
                                 AOC_T *const out) {
  if (iter->current == iter->deque->length)
    return false;
  *out = iter->deque->items[DEQUE_MOD(iter->deque->head + iter->current,
                                      iter->deque->capacity)];
  iter->current++;
  return true;
}

DEQUE_LINKAGE bool DEQUE_ITERATE_REV(DEQUE_ITER_NAME *const iter,
                                     AOC_T *const out) {
  if (iter->current == iter->deque->length)
    return false;
  i64 index = ((i64)iter->deque->tail - 1) - (i64)iter->current;
  if (index < 0)
    index = iter->deque->capacity + index;
  *out = iter->deque->items[index];
  iter->current++;
  return true;
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

#undef DEQUE_ITER_NAME
#undef DEQUE_ITER_IMPL
#undef DEQUE_ITER_INIT
#undef DEQUE_ITERATE
#undef DEQUE_ITERATE_REV
