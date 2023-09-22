#ifndef AOCAUX_COMMON_H
#define AOCAUX_COMMON_H

#include <stdint.h>
#include <stdbool.h>

#ifndef NDEBUG

#include <stdio.h>
#include <stdarg.h>
#define AOC_LOG(...) fprintf(stderr, __VA_ARGS__)

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

#endif
