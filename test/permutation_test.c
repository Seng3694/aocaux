#include <stdio.h>
#include <string.h>
#include <aoc/aoc.h>
#include <aoc/permutation.h>
#include "permutation_test_data.h"

typedef struct {
  char *data;
  size_t length;
} string;

static bool string_equals(const string *const a, const string *const b) {
  if (a->length != b->length)
    return false;
  for (size_t i = 0; i < a->length; ++i)
    if (a->data[i] != b->data[i])
      return false;

  return true;
}

// assumes hashsets work
#define AOC_T string
#define AOC_T_EMPTY ((string){0})
#define AOC_T_HASH(str) aoc_string_hash1((str)->data, (str)->length)
#define AOC_T_EQUALS string_equals
#define AOC_BASE2_CAPACITY
#include <aoc/set.h>

typedef struct {
  aoc_set_string set;
  const char *input;
} data;

static void add_to_set(const size_t *const indices, const size_t length,
                       void *userData) {
  aoc_set_string *set = userData;
  string s = {.length = length};
  s.data = aoc_calloc(length + 1, 1);
  for (size_t i = 0; i < length; ++i)
    s.data[i] = test_input[indices[i]];
  const u32 hash = aoc_string_hash1(s.data, s.length);
  assert(!aoc_set_string_contains_pre_hashed(set, s, hash));
  aoc_set_string_insert_pre_hashed(set, s, hash);
}

static void clear_set(aoc_set_string *const set) {
  aoc_set_iter_string iter = {0};
  aoc_set_iter_string_init(&iter, set);
  string key = {0};
  while (aoc_set_string_iterate(&iter, &key))
    aoc_free(key.data);
  aoc_set_string_clear(set);
}

int main(void) {
  aoc_set_string expected = {0};
  aoc_set_string_create(&expected, 1 << 12);
  for (size_t i = 0; i < AOC_ARRAY_SIZE(test_data); ++i) {
    const string key = {.data = test_data[i], .length = test_output_length};
    aoc_set_string_insert(&expected, key);
  }

  aoc_set_string actual = {0};
  aoc_set_string_create(&actual, 1 << 12);
  aoc_permutations(strlen(test_input), test_output_length, add_to_set, &actual);

  // permutations are not in lexicographic order. so use sets
  // TODO: maybe change that in the future
  assert(actual.count == expected.count);
  aoc_set_iter_string iter = {0};
  aoc_set_iter_string_init(&iter, &expected);
  string key;
  while (aoc_set_string_iterate(&iter, &key))
    assert(aoc_set_string_contains(&actual, key));

  clear_set(&actual);

  aoc_set_string_destroy(&actual);
  aoc_set_string_destroy(&expected);
}
