#include <stdlib.h>
#include "aoc/mem.h"

static inline void *aoc_alloc(void *allocator, size_t size) {
  return malloc(size);
}

static inline void *aoc_realloc(void *allocator, void *old, size_t size) {
  return realloc(old, size);
}

static inline void *aoc_calloc(void *allocator, size_t count, size_t size) {
  return calloc(count, size);
}

static inline void aoc_free(void *allocator, void *ptr) {
  return free(ptr);
}

static aoc_allocator defaultAllocator = {
    .alloc = aoc_alloc,
    .realloc = aoc_realloc,
    .calloc = aoc_calloc,
    .free = aoc_free,
    .allocator = NULL,
};
static aoc_allocator *currentAllocator = &defaultAllocator;

void AocMemSetAllocator(aoc_allocator *const allocator) {
  currentAllocator = allocator ? allocator : &defaultAllocator;
}

void *AocAlloc(AOC_SIZE_T size) {
  AOC_ASSERT(currentAllocator);
  AOC_ASSERT(currentAllocator->alloc);
  return currentAllocator->alloc(currentAllocator->allocator, size);
}

void *AocRealloc(void *old, AOC_SIZE_T size) {
  AOC_ASSERT(currentAllocator);
  AOC_ASSERT(currentAllocator->realloc);
  return currentAllocator->realloc(currentAllocator->allocator, old, size);
}

void *AocCalloc(AOC_SIZE_T count, AOC_SIZE_T size) {
  AOC_ASSERT(currentAllocator);
  AOC_ASSERT(currentAllocator->calloc);
  return currentAllocator->calloc(currentAllocator->allocator, count, size);
}

void AocFree(void *ptr) {
  AOC_ASSERT(currentAllocator);
  AOC_ASSERT(currentAllocator->free);
  currentAllocator->free(currentAllocator->allocator, ptr);
}

void AocMemCopy(void *const dest, const void *const src,
                const AOC_SIZE_T size) {
  const uint8_t *const s = (uint8_t *)src;
  uint8_t *const d = (uint8_t *)dest;
  for (AOC_SIZE_T i = 0; i < size; ++i)
    d[i] = s[i];
}
