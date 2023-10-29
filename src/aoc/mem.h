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

void aoc_set_allocator(aoc_allocator *const allocator);
void *aoc_alloc(AOC_SIZE_T size);
void *aoc_realloc(void *old, AOC_SIZE_T size);
void *aoc_calloc(AOC_SIZE_T count, AOC_SIZE_T size);
void aoc_free(void *ptr);

void aoc_mem_copy(void *const dest, const void *const src,
                  const AOC_SIZE_T size);

#endif
