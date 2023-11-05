#ifndef AOCAUX_COMMON_H
#define AOCAUX_COMMON_H

#include <stdint.h>
#include <stdbool.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

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

#define AOC_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define AOC_MIN(a, b) ((a) < (b) ? (a) : (b))
#define AOC_MAX(a, b) ((a) > (b) ? (a) : (b))

#define AOC_INC_WRAP(x, min, max) (((x) + 1) > (max) ? (min) : (x) + 1)
#define AOC_DEC_WRAP(x, min, max) (((x)-1) < (min) ? (max) : (x)-1)
#define AOC_CLAMP(x, min, max) ((x) > (max) ? (max) : (x) < (min) ? (min) : (x))

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#endif
