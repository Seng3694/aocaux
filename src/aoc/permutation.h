#ifndef AOCAUX_PERMUTATION_H
#define AOCAUX_PERMUTATION_H

#include "common.h"

typedef void (*aoc_permutation_action)(const AOC_SIZE_T *const indices,
                                       const AOC_SIZE_T length, void *userData);

void aoc_permutations(const AOC_SIZE_T inputLength,
                      const AOC_SIZE_T outputLength,
                      aoc_permutation_action action, void *userData);

#endif
