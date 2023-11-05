#include <stdio.h>
#include <string.h>
#include <aoc/combination.h>
#include "combination_rep_test_data.h"

static void check(const size_t *const indices, const size_t length,
                  void *userData) {
  size_t *index = userData;
  assert(length == test_output_length);
  char buffer[test_output_length + 1];
  buffer[test_output_length] = '\0';
  for (size_t i = 0; i < length; ++i)
    buffer[i] = test_input[indices[i]];
  assert(strcmp(buffer, test_data[*index]) == 0);
  (*index)++;
}

int main(void) {
  size_t index = 0;
  aoc_combinations(strlen(test_input), test_output_length, true, check, &index);
  assert(index == AOC_ARRAY_SIZE(test_data));
}
