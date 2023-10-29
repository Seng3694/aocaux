#ifndef AOCAUX_BUMP_H
#define AOCAUX_BUMP_H

#include "common.h"
#include "mem.h"

typedef struct {
  AOC_SIZE_T length;
  AOC_SIZE_T capacity;
#ifndef NDEBUG
  AOC_SIZE_T max;
#endif
  uintptr_t *memory;
} aoc_bump;

void *aoc_bump_alloc(aoc_bump *const bump, AOC_SIZE_T size);
void *aoc_bump_calloc(aoc_bump *const bump, AOC_SIZE_T count, AOC_SIZE_T size);
void *aoc_bump_realloc(aoc_bump *const bump, void *old, AOC_SIZE_T size);

void aoc_bump_init(aoc_bump *const bump, const AOC_SIZE_T size);
void aoc_bump_destroy(aoc_bump *const bump);
void aoc_bump_reset(aoc_bump *const bump);

aoc_allocator aoc_bump_create_allocator(aoc_bump *bump);

#endif
