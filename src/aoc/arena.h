#ifndef AOCAUX_ARENA_H
#define AOCAUX_ARENA_H

#include "common.h"
#include "mem.h"

// has to be base 2
#ifndef AOC_ARENA_DEFAULT_CAPACITY
#define AOC_ARENA_DEFAULT_CAPACITY 8192
#endif

typedef struct aoc_arena_region aoc_arena_region;

typedef struct {
  aoc_arena_region *begin;
  aoc_arena_region *end;
} aoc_arena;

void *AocArenaAlloc(aoc_arena *a, const AOC_SIZE_T size);
void *AocArenaRealloc(aoc_arena *a, void *old, const AOC_SIZE_T newSize);
void *AocArenaCalloc(aoc_arena *a, const AOC_SIZE_T count,
                     const AOC_SIZE_T size);
void AocArenaReset(aoc_arena *a);
void AocArenaFree(aoc_arena *a);

aoc_allocator AocArenaCreateAllocator(aoc_arena *a);

#endif
