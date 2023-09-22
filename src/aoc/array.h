#include "mem.h"

#ifndef AOC_T
#error "AOC_T must be defined"
#define AOC_T int
#endif

#ifndef AOC_T_NAME
#error "AOC_T_NAME must be defined"
#define AOC_T_NAME Int
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define ARRAY_NAME COMBINE2(AocArray, AOC_T_NAME)
#define ARRAY_IMPL(word) COMBINE2(ARRAY_NAME, word)
#define ARRAY_LINKAGE static inline

typedef struct {
  AOC_SIZE_T capacity;
  AOC_SIZE_T length;
  AOC_T *items;
} ARRAY_NAME;

#define ARRAY_CREATE ARRAY_IMPL(Create)
#define ARRAY_DESTROY ARRAY_IMPL(Destroy)
#define ARRAY_COPY ARRAY_IMPL(Copy)
#define ARRAY_DUPE ARRAY_IMPL(Duplicate)
#define ARRAY_PUSH ARRAY_IMPL(Push)
#define ARRAY_POP ARRAY_IMPL(Pop)
#define ARRAY_FIRST ARRAY_IMPL(First)
#define ARRAY_LAST ARRAY_IMPL(Last)
#define ARRAY_ENSURE_CAPACITY ARRAY_IMPL(EnsureCapacity)
#define ARRAY_RESIZE ARRAY_IMPL(Resize)
#define ARRAY_CLEAR ARRAY_IMPL(Clear)

ARRAY_LINKAGE void ARRAY_CREATE(ARRAY_NAME *const arr,
                                const AOC_SIZE_T capacity);
ARRAY_LINKAGE void ARRAY_DESTROY(ARRAY_NAME *const arr);
ARRAY_LINKAGE void ARRAY_COPY(ARRAY_NAME *const dest,
                              const ARRAY_NAME *const src);
ARRAY_LINKAGE void ARRAY_DUPE(ARRAY_NAME *const dest,
                              const ARRAY_NAME *const src);
ARRAY_LINKAGE void ARRAY_PUSH(ARRAY_NAME *const arr, AOC_T item);
ARRAY_LINKAGE void ARRAY_POP(ARRAY_NAME *const arr);
ARRAY_LINKAGE AOC_T *ARRAY_FIRST(ARRAY_NAME *const arr);
ARRAY_LINKAGE AOC_T *ARRAY_LAST(ARRAY_NAME *const arr);
ARRAY_LINKAGE void ARRAY_ENSURE_CAPACITY(ARRAY_NAME *const arr,
                                         const AOC_SIZE_T capacity);
ARRAY_LINKAGE void ARRAY_RESIZE(ARRAY_NAME *const arr,
                                const AOC_SIZE_T capacity);
ARRAY_LINKAGE void ARRAY_CLEAR(ARRAY_NAME *const arr);

ARRAY_LINKAGE void ARRAY_CREATE(ARRAY_NAME *const arr,
                                const AOC_SIZE_T capacity) {
  arr->capacity = capacity;
  arr->length = 0;
  arr->items = (AOC_T *)AocAlloc(sizeof(AOC_T) * capacity);
}

ARRAY_LINKAGE void ARRAY_DESTROY(ARRAY_NAME *const arr) {
  AocFree(arr->items);
  arr->items = NULL;
}

ARRAY_LINKAGE void ARRAY_COPY(ARRAY_NAME *const dest,
                              const ARRAY_NAME *const src) {
  ARRAY_ENSURE_CAPACITY(dest, src->capacity);
  AocMemCopy(dest->items, src->items, src->length * sizeof(AOC_T));
  dest->length = src->length;
}

ARRAY_LINKAGE void ARRAY_DUPE(ARRAY_NAME *const dest,
                              const ARRAY_NAME *const src) {
  ARRAY_CREATE(dest, src->capacity);
  AocMemCopy(dest->items, src->items, src->length * sizeof(AOC_T));
  dest->length = src->length;
}

ARRAY_LINKAGE void ARRAY_PUSH(ARRAY_NAME *const arr, AOC_T item) {
  ARRAY_ENSURE_CAPACITY(arr, arr->length + 1);
  arr->items[arr->length] = item;
  arr->length++;
}

ARRAY_LINKAGE void ARRAY_POP(ARRAY_NAME *const arr) {
  if (arr->length == 0)
    return;
  arr->length--;
}

ARRAY_LINKAGE AOC_T *ARRAY_FIRST(ARRAY_NAME *const arr) {
  return arr->length > 0 ? &arr->items[0] : NULL;
}

ARRAY_LINKAGE AOC_T *ARRAY_LAST(ARRAY_NAME *const arr) {
  return arr->length > 0 ? &arr->items[arr->length - 1] : NULL;
}

ARRAY_LINKAGE void ARRAY_ENSURE_CAPACITY(ARRAY_NAME *const arr,
                                         const AOC_SIZE_T capacity) {
  if (arr->capacity < capacity) {
    ARRAY_RESIZE(arr, arr->capacity * 2);
  }
}
ARRAY_LINKAGE void ARRAY_RESIZE(ARRAY_NAME *const arr,
                                const AOC_SIZE_T capacity) {
  arr->capacity = capacity;
  arr->items = (AOC_T *)AocRealloc(arr->items, sizeof(AOC_T) * capacity);
}

ARRAY_LINKAGE void ARRAY_CLEAR(ARRAY_NAME *const arr) {
  arr->length = 0;
}

#undef COMBINE
#undef COMBINE2
#undef AOC_T
#undef AOC_T_NAME
#undef ARRAY_NAME
#undef ARRAY_IMPL
#undef ARRAY_LINKAGE
#undef ARRAY_CREATE
#undef ARRAY_DESTROY
#undef ARRAY_COPY
#undef ARRAY_DUPE
#undef ARRAY_PUSH
#undef ARRAY_POP
#undef ARRAY_FIRST
#undef ARRAY_LAST
#undef ARRAY_ENSURE_CAPACITY
#undef ARRAY_RESIZE
#undef ARRAY_CLEAR
