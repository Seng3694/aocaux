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

#define VECTOR_NAME COMBINE2(aoc_vector_, AOC_T_NAME)
#define VECTOR_IMPL(word) COMBINE2(VECTOR_NAME, COMBINE2(_, word))
#define VECTOR_LINKAGE static inline

typedef struct {
  AOC_SIZE_T capacity;
  AOC_SIZE_T length;
  AOC_T *items;
} VECTOR_NAME;

#define VECTOR_CREATE VECTOR_IMPL(create)
#define VECTOR_DESTROY VECTOR_IMPL(destroy)
#define VECTOR_COPY VECTOR_IMPL(copy)
#define VECTOR_DUPE VECTOR_IMPL(duplicate)
#define VECTOR_PUSH VECTOR_IMPL(push)
#define VECTOR_POP VECTOR_IMPL(pop)
#define VECTOR_FIRST VECTOR_IMPL(first)
#define VECTOR_LAST VECTOR_IMPL(last)
#define VECTOR_ENSURE_CAPACITY VECTOR_IMPL(ensure_capacity)
#define VECTOR_RESIZE VECTOR_IMPL(resize)
#define VECTOR_CLEAR VECTOR_IMPL(clear)

#define VECTOR_REMOVE VECTOR_IMPL(remove)
#define VECTOR_REMOVE_SWP VECTOR_IMPL(remove_swp)
#define VECTOR_PUSH_MANY VECTOR_IMPL(push_many)

VECTOR_LINKAGE void VECTOR_CREATE(VECTOR_NAME *const vec,
                                  const AOC_SIZE_T capacity);
VECTOR_LINKAGE void VECTOR_DESTROY(VECTOR_NAME *const vec);
VECTOR_LINKAGE void VECTOR_COPY(VECTOR_NAME *const dest,
                                const VECTOR_NAME *const src);
VECTOR_LINKAGE void VECTOR_DUPE(VECTOR_NAME *const dest,
                                const VECTOR_NAME *const src);
VECTOR_LINKAGE void VECTOR_PUSH(VECTOR_NAME *const vec, AOC_T item);
VECTOR_LINKAGE void VECTOR_POP(VECTOR_NAME *const vec);
VECTOR_LINKAGE AOC_T *VECTOR_FIRST(VECTOR_NAME *const vec);
VECTOR_LINKAGE AOC_T *VECTOR_LAST(VECTOR_NAME *const vec);
VECTOR_LINKAGE void VECTOR_ENSURE_CAPACITY(VECTOR_NAME *const vec,
                                           const AOC_SIZE_T capacity);
VECTOR_LINKAGE void VECTOR_RESIZE(VECTOR_NAME *const vec,
                                  const AOC_SIZE_T capacity);
VECTOR_LINKAGE void VECTOR_CLEAR(VECTOR_NAME *const vec);
VECTOR_LINKAGE void VECTOR_REMOVE(VECTOR_NAME *const vec,
                                  const AOC_SIZE_T index);
VECTOR_LINKAGE void VECTOR_REMOVE_SWP(VECTOR_NAME *const vec,
                                      const AOC_SIZE_T index);
VECTOR_LINKAGE void VECTOR_PUSH_MANY(VECTOR_NAME *const vec,
                                     const AOC_T *const src,
                                     const AOC_SIZE_T count);

VECTOR_LINKAGE void VECTOR_CREATE(VECTOR_NAME *const vec,
                                  const AOC_SIZE_T capacity) {
  vec->capacity = capacity;
  vec->length = 0;
  vec->items = (AOC_T *)aoc_alloc(sizeof(AOC_T) * capacity);
}

VECTOR_LINKAGE void VECTOR_DESTROY(VECTOR_NAME *const vec) {
  aoc_free(vec->items);
  vec->items = NULL;
}

VECTOR_LINKAGE void VECTOR_COPY(VECTOR_NAME *const dest,
                                const VECTOR_NAME *const src) {
  VECTOR_ENSURE_CAPACITY(dest, src->capacity);
  aoc_mem_copy(dest->items, src->items, src->length * sizeof(AOC_T));
  dest->length = src->length;
}

VECTOR_LINKAGE void VECTOR_DUPE(VECTOR_NAME *const dest,
                                const VECTOR_NAME *const src) {
  VECTOR_CREATE(dest, src->capacity);
  aoc_mem_copy(dest->items, src->items, src->length * sizeof(AOC_T));
  dest->length = src->length;
}

VECTOR_LINKAGE void VECTOR_PUSH(VECTOR_NAME *const vec, AOC_T item) {
  VECTOR_ENSURE_CAPACITY(vec, vec->length + 1);
  vec->items[vec->length] = item;
  vec->length++;
}

VECTOR_LINKAGE void VECTOR_POP(VECTOR_NAME *const vec) {
  if (vec->length == 0)
    return;
  vec->length--;
}

VECTOR_LINKAGE AOC_T *VECTOR_FIRST(VECTOR_NAME *const vec) {
  return vec->length > 0 ? &vec->items[0] : NULL;
}

VECTOR_LINKAGE AOC_T *VECTOR_LAST(VECTOR_NAME *const vec) {
  return vec->length > 0 ? &vec->items[vec->length - 1] : NULL;
}

VECTOR_LINKAGE void VECTOR_ENSURE_CAPACITY(VECTOR_NAME *const vec,
                                           const AOC_SIZE_T capacity) {
  if (vec->capacity < capacity) {
    VECTOR_RESIZE(vec, vec->capacity * 2);
  }
}
VECTOR_LINKAGE void VECTOR_RESIZE(VECTOR_NAME *const vec,
                                  const AOC_SIZE_T capacity) {
  vec->capacity = capacity;
  vec->items = (AOC_T *)aoc_realloc(vec->items, sizeof(AOC_T) * capacity);
}

VECTOR_LINKAGE void VECTOR_CLEAR(VECTOR_NAME *const vec) {
  vec->length = 0;
}

VECTOR_LINKAGE void VECTOR_REMOVE(VECTOR_NAME *const vec,
                                  const AOC_SIZE_T index) {
  AOC_ASSERT(index < vec->length);
  for (AOC_SIZE_T i = index; i < vec->length - 2; ++i)
    vec->items[i] = vec->items[i + 1];
  vec->length--;
}

VECTOR_LINKAGE void VECTOR_REMOVE_SWP(VECTOR_NAME *const vec,
                                      const AOC_SIZE_T index) {
  AOC_ASSERT(index < vec->length);
  vec->items[index] = vec->items[--vec->length];
}

VECTOR_LINKAGE void VECTOR_PUSH_MANY(VECTOR_NAME *const vec,
                                     const AOC_T *const src,
                                     const AOC_SIZE_T count) {
  VECTOR_ENSURE_CAPACITY(vec, vec->length + count);
  for (AOC_SIZE_T i = 0; i < count; ++i)
    vec->items[vec->length++] = src[i];
}

#undef COMBINE
#undef COMBINE2
#undef AOC_T
#undef AOC_T_NAME
#undef VECTOR_NAME
#undef VECTOR_IMPL
#undef VECTOR_LINKAGE
#undef VECTOR_CREATE
#undef VECTOR_DESTROY
#undef VECTOR_COPY
#undef VECTOR_DUPE
#undef VECTOR_PUSH
#undef VECTOR_POP
#undef VECTOR_FIRST
#undef VECTOR_LAST
#undef VECTOR_ENSURE_CAPACITY
#undef VECTOR_RESIZE
#undef VECTOR_CLEAR
#undef VECTOR_REMOVE
#undef VECTOR_REMOVE_SWP
#undef VECTOR_PUSH_MANY
