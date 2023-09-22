#include <stdlib.h>

#include "aoc/arena.h"
#include "aoc/common.h"

typedef struct aoc_arena_region {
  struct aoc_arena_region *next;
  AOC_SIZE_T count;
  AOC_SIZE_T capacity;
  uintptr_t data[];
} aoc_arena_region;

#define ALIGN_TO_PTR_SIZE(size)                                                \
  (size + sizeof(uintptr_t) - 1) & (~(sizeof(uintptr_t) - 1))

static aoc_arena_region *aoc_arena_region_new(const AOC_SIZE_T capacity) {
  const AOC_SIZE_T bytes =
      sizeof(aoc_arena_region) + sizeof(uintptr_t) * capacity;
  aoc_arena_region *r = malloc(bytes);
  AOC_LOG("aoc_arena new region allocated\n");
  AOC_ASSERT(r);
  r->next = NULL;
  r->count = 0;
  r->capacity = capacity;
  return r;
}

static void aoc_arena_region_free(aoc_arena_region *r) {
  free(r);
}

static void *aoc_arena_alloc_internal(aoc_arena *a, const AOC_SIZE_T size) {
  // add one extra slot for the block size (header)
  // [size][  requested memory block  ]
  const AOC_SIZE_T s = ALIGN_TO_PTR_SIZE(size + sizeof(uintptr_t));
  if (a->end == NULL) {
    AOC_ASSERT(a->begin == NULL);
    AOC_SIZE_T capacity = AOC_ARENA_DEFAULT_CAPACITY;
    if (capacity < s)
      capacity = s;
    a->end = aoc_arena_region_new(capacity);
    a->begin = a->end;
  }

  while (a->end->count + s > a->end->capacity && a->end->next != NULL)
    a->end = a->end->next;

  if (a->end->count + s > a->end->capacity) {
    AOC_ASSERT(a->end->next == NULL);
    AOC_SIZE_T capacity = AOC_ARENA_DEFAULT_CAPACITY;
    if (capacity < s)
      capacity = s;
    a->end->next = aoc_arena_region_new(capacity);
    a->end = a->end->next;
  }

  a->end->data[a->end->count] = (uintptr_t)size;
  void *result = &a->end->data[a->end->count + 1];
  a->end->count += s;
  return result;
}

void *AocArenaAlloc(aoc_arena *a, const AOC_SIZE_T size) {
  AOC_LOG("aoc_arena_alloc '%zu' bytes\n", size);
  return aoc_arena_alloc_internal(a, size);
}

void *AocArenaRealloc(aoc_arena *a, void *old, const AOC_SIZE_T newSize) {
  if (old == NULL) {
    AOC_LOG("aoc_arena_realloc '%zu' bytes (with NULL)\n", newSize);
    return aoc_arena_alloc_internal(a, newSize);
  }

  const AOC_SIZE_T oldSize = (AOC_SIZE_T)(*(((uintptr_t *)old) - 1));
  AOC_LOG("aoc_arena_realloc '%zu' => '%zu' bytes %s\n", oldSize, newSize,
          oldSize >= newSize ? "(reuse)" : "");
  if (oldSize >= newSize) {
    return old;
  }
  char *oldData = old;
  char *newData = aoc_arena_alloc_internal(a, newSize);
  for (AOC_SIZE_T i = 0; i < oldSize; ++i)
    newData[i] = oldData[i];
  return newData;
}

void *AocArenaCalloc(aoc_arena *a, const AOC_SIZE_T count,
                     const AOC_SIZE_T size) {
  AOC_LOG("aoc_arena_calloc count='%zu' size='%zu' (%zu bytes)\n", count, size,
          count * size);
  const AOC_SIZE_T s = count * size;
  char *data = aoc_arena_alloc_internal(a, s);
  for (AOC_SIZE_T i = 0; i < s; ++i)
    data[i] = 0;
  return data;
}

void AocArenaReset(aoc_arena *a) {
  for (aoc_arena_region *r = a->begin; r != NULL; r = r->next)
    r->count = 0;
  a->end = a->begin;
}

void AocArenaFree(aoc_arena *a) {
#ifndef NDEBUG
  AOC_SIZE_T used = 0;
  AOC_SIZE_T total = 0;
#endif
  aoc_arena_region *r = a->begin;
  while (r) {
    aoc_arena_region *tmp = r;
    r = r->next;
#ifndef NDEBUG
    used += tmp->count;
    total += tmp->capacity;
#endif
    aoc_arena_region_free(tmp);
  }
  a->begin = NULL;
  a->end = NULL;

  AOC_LOG("aoc_arena memory freed:\n allocated %zu\n used %zu (%.2f)\n", total,
          used, (used / (float)total) * 100);
}

static inline void *aoc_arena_alloc(void *allocator, AOC_SIZE_T size) {
  return AocArenaAlloc(allocator, size);
}

static inline void *aoc_arena_realloc(void *allocator, void *old,
                                      AOC_SIZE_T size) {
  return AocArenaRealloc(allocator, old, size);
}

static inline void *aoc_arena_calloc(void *allocator, AOC_SIZE_T count,
                                     AOC_SIZE_T size) {
  return AocArenaCalloc(allocator, count, size);
}

static inline void aoc_arena_free(void *allocator, void *ptr) {
  (void)allocator;
  (void)ptr;
}

aoc_allocator AocArenaCreateAllocator(aoc_arena *a) {
  return (aoc_allocator){
      .alloc = aoc_arena_alloc,
      .calloc = aoc_arena_calloc,
      .realloc = aoc_arena_realloc,
      .free = aoc_arena_free,
      .allocator = a,
  };
}
