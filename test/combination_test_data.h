#include <stddef.h>
static const char test_input[] = "abcdef";
static const size_t test_output_length = 4;
static char *test_data[] = {
  "abcd",
  "abce",
  "abcf",
  "abde",
  "abdf",
  "abef",
  "acde",
  "acdf",
  "acef",
  "adef",
  "bcde",
  "bcdf",
  "bcef",
  "bdef",
  "cdef",
};
