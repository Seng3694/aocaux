#include "mem.h"

#ifndef AOC_T
#error "AOC_T must be defined"
#define AOC_T int
#endif

#ifndef AOC_T_NAME
#define AOC_T_NAME AOC_T
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define ARRAY_NAME COMBINE2(aoc_array_, AOC_T_NAME)
#define ARRAY_IMPL(word) COMBINE2(ARRAY_NAME, COMBINE2(_, word))
#define ARRAY_LINKAGE static inline

typedef struct {
  AOC_SIZE_T capacity;
  AOC_SIZE_T length;
  AOC_T *items;
} ARRAY_NAME;

#define ARRAY_CREATE ARRAY_IMPL(create)
#define ARRAY_DESTROY ARRAY_IMPL(destroy)
#define ARRAY_COPY ARRAY_IMPL(copy)
#define ARRAY_DUPE ARRAY_IMPL(duplicate)
#define ARRAY_PUSH ARRAY_IMPL(push)
#define ARRAY_POP ARRAY_IMPL(pop)
#define ARRAY_FIRST ARRAY_IMPL(first)
#define ARRAY_LAST ARRAY_IMPL(last)
#define ARRAY_ENSURE_CAPACITY ARRAY_IMPL(ensure_capacity)
#define ARRAY_RESIZE ARRAY_IMPL(resize)
#define ARRAY_CLEAR ARRAY_IMPL(clear)

#define ARRAY_REMOVE ARRAY_IMPL(remove)
#define ARRAY_REMOVE_SWP ARRAY_IMPL(remove_swp)
#define ARRAY_PUSH_MANY ARRAY_IMPL(push_many)

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
ARRAY_LINKAGE void ARRAY_REMOVE(ARRAY_NAME *const arr, const AOC_SIZE_T index);
ARRAY_LINKAGE void ARRAY_REMOVE_SWP(ARRAY_NAME *const arr,
                                    const AOC_SIZE_T index);
ARRAY_LINKAGE void ARRAY_PUSH_MANY(ARRAY_NAME *const arr,
                                   const AOC_T const *src,
                                   const AOC_SIZE_T count);

ARRAY_LINKAGE void ARRAY_CREATE(ARRAY_NAME *const arr,
                                const AOC_SIZE_T capacity) {
  arr->capacity = capacity;
  arr->length = 0;
  arr->items = (AOC_T *)aoc_alloc(sizeof(AOC_T) * capacity);
}

ARRAY_LINKAGE void ARRAY_DESTROY(ARRAY_NAME *const arr) {
  aoc_free(arr->items);
  arr->items = NULL;
}

ARRAY_LINKAGE void ARRAY_COPY(ARRAY_NAME *const dest,
                              const ARRAY_NAME *const src) {
  ARRAY_ENSURE_CAPACITY(dest, src->capacity);
  aoc_mem_copy(dest->items, src->items, src->length * sizeof(AOC_T));
  dest->length = src->length;
}

ARRAY_LINKAGE void ARRAY_DUPE(ARRAY_NAME *const dest,
                              const ARRAY_NAME *const src) {
  ARRAY_CREATE(dest, src->capacity);
  aoc_mem_copy(dest->items, src->items, src->length * sizeof(AOC_T));
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
  arr->items = (AOC_T *)aoc_realloc(arr->items, sizeof(AOC_T) * capacity);
}

ARRAY_LINKAGE void ARRAY_CLEAR(ARRAY_NAME *const arr) {
  arr->length = 0;
}

ARRAY_LINKAGE void ARRAY_REMOVE(ARRAY_NAME *const arr, const AOC_SIZE_T index) {
  AOC_ASSERT(index < arr->length);
  for (AOC_SIZE_T i = index; i < arr->length - 2; ++i)
    arr->items[i] = arr->items[i + 1];
  arr->length--;
}

ARRAY_LINKAGE void ARRAY_REMOVE_SWP(ARRAY_NAME *const arr,
                                    const AOC_SIZE_T index) {
  AOC_ASSERT(index < arr->length);
  arr->items[index] = arr->items[--arr->length];
}

ARRAY_LINKAGE void ARRAY_PUSH_MANY(ARRAY_NAME *const arr,
                                   const AOC_T const *src,
                                   const AOC_SIZE_T count) {
  ARRAY_ENSURE_CAPACITY(arr, arr->length + count);
  for (AOC_SIZE_T i = 0; i < count; ++i)
    arr->items[arr->length++] = src[i];
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
#undef ARRAY_REMOVE
#undef ARRAY_REMOVE_SWP
#undef ARRAY_PUSH_MANY
