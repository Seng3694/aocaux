#include "aoc/common.h"
#include "aoc/arena.h"

static aoc_arena *currentArena = NULL;

void AocSetArena(aoc_arena *const arena) {
  AOC_ASSERT(arena != NULL);
  currentArena = arena;
}

void *AocMalloc(AOC_SIZE_T size) {
  AOC_ASSERT(currentArena != NULL);
  return AocArenaAlloc(currentArena, size);
}

void AocFree(void *ptr) {
  (void)ptr;
}

void *AocCalloc(AOC_SIZE_T count, AOC_SIZE_T size) {
  AOC_ASSERT(currentArena != NULL);
  return AocArenaCalloc(currentArena, count, size);
}

void *AocRealloc(void *oldPtr, AOC_SIZE_T newSize) {
  AOC_ASSERT(currentArena != NULL);
  return AocArenaRealloc(currentArena, oldPtr, newSize);
}
