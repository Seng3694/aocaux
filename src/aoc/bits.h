#ifndef AOCAUX_BITS_H
#define AOCAUX_BITS_H

#include "common.h"

#define AOC_CLEAR_BIT(value, bit) ((value) & ~(1 << (bit)))
#define AOC_SET_BIT(value, bit) ((value) | (1 << (bit)))
#define AOC_TOGGLE_BIT(value, bit) ((value) ^ (1 << (bit)))
#define AOC_CHECK_BIT(value, bit) (((value) & (1 << (bit))) == (1 << (bit)))

uint32_t AocPopCnt32(const uint32_t i);

#endif
