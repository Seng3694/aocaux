#ifndef AOCAUX_COMBINATION_H
#define AOCAUX_COMBINATION_H

#include "common.h"

typedef void (*aoc_combination_action)(const AOC_SIZE_T *const indices,
                                       const AOC_SIZE_T length, void *userData);

void aoc_combinations(const AOC_SIZE_T inputLength,
                      const AOC_SIZE_T outputLength, const bool repetitions,
                      aoc_combination_action action, void *userData);

#endif
