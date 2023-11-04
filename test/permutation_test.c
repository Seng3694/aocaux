#include <stdio.h>
#include <aoc/aoc.h>
#include <aoc/permutation.h>

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
  data *d = userData;
  string s = {.length = length};
  s.data = aoc_calloc(length + 1, 1);
  for (size_t i = 0; i < length; ++i)
    s.data[i] = d->input[indices[i]];
  const u32 hash = aoc_string_hash1(s.data, s.length);
  assert(!aoc_set_string_contains_pre_hashed(&d->set, s, hash));
  aoc_set_string_insert_pre_hashed(&d->set, s, hash);
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
  data d = {.input = "abcdef"};
  aoc_set_string_create(&d.set, 1 << 12);
  // P(n,r) = n!/(n-r)!
  // 6!/(6-4)! = 360
  aoc_permute(6, 4, add_to_set, &d);
  assert(d.set.count == 360);
  clear_set(&d.set);

  // 6!/(6-6)! = 720
  aoc_permute(6, 6, add_to_set, &d);
  assert(d.set.count == 720);

  clear_set(&d.set);
  aoc_set_string_destroy(&d.set);
}
