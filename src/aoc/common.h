#ifndef AOCAUX_COMMON_H
#define AOCAUX_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef NDEBUG

#include <stdio.h>
#include <stdarg.h>
#define AOC_LOG(...) printf(__VA_ARGS__)
#else
#define AOC_LOG(...)
#endif

#endif
