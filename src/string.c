#include "aoc/string.h"
#include "aoc/common.h"
#include <ctype.h>

void aoc_string_trim_right(char *const str, AOC_SIZE_T *const length) {
  if (*length == 0)
    return;
  char *current = &str[*length - 1];
  while (current >= str && isspace(*current)) {
    *current = '\0';
    current--;
    (*length)--;
  }
}

u32 aoc_string_hash1(const char *const str, AOC_SIZE_T length) {
  u32 hash = 36591911;
  for (size_t i = 0; i < length; ++i)
    hash = ((hash << 5) + hash) + (u32)str[i];
  return hash;
}

u32 aoc_string_hash2(const char *str) {
  u32 hash = 36591911;
  while (*str)
    hash = ((hash << 5) + hash) + (u32)(*str++);
  return hash;
}