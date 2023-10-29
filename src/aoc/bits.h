#ifndef AOCAUX_BITS_H
#define AOCAUX_BITS_H

#include "common.h"

#define AOC_CLEAR_BIT(value, bit) ((value) & ~(1 << (bit)))
#define AOC_SET_BIT(value, bit) ((value) | (1 << (bit)))
#define AOC_TOGGLE_BIT(value, bit) ((value) ^ (1 << (bit)))
#define AOC_CHECK_BIT(value, bit) (((value) & (1 << (bit))) == (1 << (bit)))
#define AOC_HAS_FLAG(flags, flag) (((flags) & (flag)) == (flag))

#if __has_builtin(__builtin_popcount)
inline u32 aoc_popcount(const u32 i) {
  return (u32)__builtin_popcount(i);
}
#else
u32 aoc_popcount(const u32 i);
#endif

#endif
