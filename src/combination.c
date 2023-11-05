#include "aoc/combination.h"

static void combinations(const AOC_SIZE_T inputLength,
                         const AOC_SIZE_T outputLength,
                         AOC_SIZE_T *const indices,
                         const AOC_SIZE_T indicesLength, const AOC_SIZE_T start,
                         const AOC_SIZE_T noRepetions,
                         aoc_combination_action action, void *userData) {
  if (indicesLength == outputLength) {
    action(indices, outputLength, userData);
    return;
  }

  for (AOC_SIZE_T i = start; i < inputLength; ++i) {
    indices[indicesLength] = i;
    combinations(inputLength, outputLength, indices, indicesLength + 1,
                 i + noRepetions, noRepetions, action, userData);
  }
}

void aoc_combinations(const AOC_SIZE_T inputLength,
                      const AOC_SIZE_T outputLength, const bool repetitions,
                      aoc_combination_action action, void *userData) {
  AOC_SIZE_T indices[outputLength];
  combinations(inputLength, outputLength, indices, 0, 0, repetitions ? 0 : 1,
               action, userData);
}
