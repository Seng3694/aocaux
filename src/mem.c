#include <stdlib.h>
#include "aoc/mem.h"

static inline void *alloc_default(void *allocator, AOC_SIZE_T size) {
  return malloc(size);
}

static inline void *realloc_default(void *allocator, void *old,
                                    AOC_SIZE_T size) {
  return realloc(old, size);
}

static inline void *calloc_default(void *allocator, AOC_SIZE_T count,
                                   AOC_SIZE_T size) {
  return calloc(count, size);
}

static inline void free_default(void *allocator, void *ptr) {
  return free(ptr);
}

static aoc_allocator defaultAllocator = {
    .alloc = alloc_default,
    .realloc = realloc_default,
    .calloc = calloc_default,
    .free = free_default,
    .allocator = NULL,
};
static aoc_allocator *currentAllocator = &defaultAllocator;

void aoc_set_allocator(aoc_allocator *const allocator) {
  currentAllocator = allocator ? allocator : &defaultAllocator;
}

void *aoc_alloc(AOC_SIZE_T size) {
  AOC_ASSERT(currentAllocator);
  AOC_ASSERT(currentAllocator->alloc);
  return currentAllocator->alloc(currentAllocator->allocator, size);
}

void *aoc_realloc(void *old, AOC_SIZE_T size) {
  AOC_ASSERT(currentAllocator);
  AOC_ASSERT(currentAllocator->realloc);
  return currentAllocator->realloc(currentAllocator->allocator, old, size);
}

void *aoc_calloc(AOC_SIZE_T count, AOC_SIZE_T size) {
  AOC_ASSERT(currentAllocator);
  AOC_ASSERT(currentAllocator->calloc);
  return currentAllocator->calloc(currentAllocator->allocator, count, size);
}

void aoc_free(void *ptr) {
  AOC_ASSERT(currentAllocator);
  AOC_ASSERT(currentAllocator->free);
  currentAllocator->free(currentAllocator->allocator, ptr);
}

void aoc_mem_copy(void *const dest, const void *const src,
                  const AOC_SIZE_T size) {
  const u8 *const s = (u8 *)src;
  u8 *const d = (u8 *)dest;
  for (AOC_SIZE_T i = 0; i < size; ++i)
    d[i] = s[i];
}
