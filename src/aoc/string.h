#ifndef AOCAUX_STRING_H
#define AOCAUX_STRING_H

#include "common.h"

void aoc_string_trim_right(char *const str, AOC_SIZE_T *const length);
u32 aoc_string_hash1(const char *const str, AOC_SIZE_T length);
u32 aoc_string_hash2(const char *str);

#endif
