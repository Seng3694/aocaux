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

#ifndef AOC_MALLOC
#include <stdlib.h>
#define AOC_MALLOC malloc
#endif

#ifndef AOC_FREE
#include <stdlib.h>
#define AOC_FREE free
#endif

#ifndef AOC_CALLOC
#include <stdlib.h>
#define AOC_CALLOC calloc
#endif

#ifndef AOC_REALLOC
#include <stdlib.h>
#define AOC_REALLOC realloc
#endif

#ifndef AOC_SIZE_T
#include <stddef.h>
#define AOC_SIZE_T size_t
#endif

#endif
