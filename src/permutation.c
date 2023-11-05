#include "aoc/permutation.h"

static inline void swap(AOC_SIZE_T *const a, AOC_SIZE_T *const b) {
  const AOC_SIZE_T tmp = *a;
  *a = *b;
  *b = tmp;
}

static void permutations(AOC_SIZE_T *indices, const AOC_SIZE_T outputLength,
                         const AOC_SIZE_T start, const AOC_SIZE_T end,
                         aoc_permutation_action action, void *userData) {
  if (start == outputLength) {
    action(indices, outputLength, userData);
  } else {
    for (AOC_SIZE_T i = start; i <= end; ++i) {
      swap(&indices[start], &indices[i]);
      permutations(indices, outputLength, start + 1, end, action, userData);
      swap(&indices[start], &indices[i]);
    }
  }
}

void aoc_permutations(const AOC_SIZE_T inputLength,
                      const AOC_SIZE_T outputLength,
                      aoc_permutation_action action, void *userData) {
  AOC_SIZE_T indices[inputLength];
  for (AOC_SIZE_T i = 0; i < inputLength; ++i)
    indices[i] = i;
  permutations(indices, outputLength, 0, inputLength - 1, action, userData);
}
