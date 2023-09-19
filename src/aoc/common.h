#ifndef AOCAUX_COMMON_H
#define AOCAUX_COMMON_H

#include <stdint.h>
#include <stdbool.h>

#ifndef NDEBUG

#include <stdio.h>
#include <stdarg.h>
#define AOC_LOG(...) printf(__VA_ARGS__)

#include <assert.h>
#define AOC_ASSERT(expr) assert(expr)

#else
#define AOC_LOG(...)
#define AOC_ASSERT(expr)
#endif

#ifndef AOC_SIZE_T
#include <stddef.h>
#define AOC_SIZE_T size_t
#endif

#ifdef AOC_USE_ARENA_DEFAULT
#include "arena.h"
void AocSetArena(aoc_arena *const arena);
void *AocMalloc(AOC_SIZE_T size);
void AocFree(void *ptr);
void *AocCalloc(AOC_SIZE_T count, AOC_SIZE_T size);
void *AocRealloc(void *oldPtr, AOC_SIZE_T newSize);
#endif

#ifndef AOC_MALLOC
#ifdef AOC_USE_ARENA_DEFAULT
#define AOC_MALLOC AocMalloc
#else
#include <stdlib.h>
#define AOC_MALLOC malloc
#endif
#endif

#ifndef AOC_FREE
#ifdef AOC_USE_ARENA_DEFAULT
#define AOC_FREE AocFree
#else
#include <stdlib.h>
#define AOC_FREE free
#endif
#endif

#ifndef AOC_CALLOC
#ifdef AOC_USE_ARENA_DEFAULT
#define AOC_CALLOC AocCalloc
#else
#include <stdlib.h>
#define AOC_CALLOC calloc
#endif
#endif

#ifndef AOC_REALLOC
#ifdef AOC_USE_ARENA_DEFAULT
#define AOC_REALLOC AocRealloc
#else
#include <stdlib.h>
#define AOC_REALLOC realloc
#endif
#endif

#endif
