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

void *AocBumpAlloc(aoc_bump *const bump, const AOC_SIZE_T size);
void AocBumpInit(aoc_bump *const bump, const AOC_SIZE_T size);
void AocBumpDestroy(aoc_bump *const bump);
void AocBumpReset(aoc_bump *const bump);

aoc_allocator AocBumpCreateAllocator(aoc_bump *bump);

#endif
