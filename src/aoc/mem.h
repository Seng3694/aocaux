#ifndef AOC_MEM_H
#define AOC_MEM_H

#include "common.h"

typedef void *(*aoc_alloc_func)(void *allocator, AOC_SIZE_T size);
typedef void *(*aoc_realloc_func)(void *allocator, void *old,
                                  AOC_SIZE_T newSize);
typedef void *(*aoc_calloc_func)(void *allocator, AOC_SIZE_T count,
                                 AOC_SIZE_T size);
typedef void (*aoc_free_func)(void *allocator, void *ptr);

typedef struct {
  aoc_alloc_func alloc;
  aoc_realloc_func realloc;
  aoc_calloc_func calloc;
  aoc_free_func free;
  void *allocator;
} aoc_allocator;

void AocMemSetAllocator(aoc_allocator *const allocator);
void *AocAlloc(AOC_SIZE_T size);
void *AocRealloc(void *old, AOC_SIZE_T size);
void *AocCalloc(AOC_SIZE_T count, AOC_SIZE_T size);
void AocFree(void *ptr);

void AocMemCopy(void *const dest, const void *const src, const AOC_SIZE_T size);

#endif
