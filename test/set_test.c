#include <aoc/aoc.h>
#include <stdio.h>
#include <stdlib.h>

#define AOC_T i32
#define AOC_T_EMPTY ((i32){INT32_MIN})
#define AOC_T_HASH(x) ((((u32)(*(x))) * 36591911) ^ 75656261)
#define AOC_T_EQUALS(a, b) (*(a) == *(b))
#define AOC_BASE2_CAPACITY
#include <aoc/set.h>

static inline int compare_i32(const void *const a, const void *const b) {
  return *(i32 *)a - *(i32 *)b;
}

int main(void) {
  aoc_set_i32 set = {0};
  aoc_set_i32_create(&set, 32);

  assert(set.count == 0);
  assert(set.capacity == 32);

  // insert odd numbers
  for (i32 i = 0; i < 10; ++i)
    if (i % 2 != 0)
      aoc_set_i32_insert(&set, i);

  assert(set.count == 5);
  assert(set.capacity == 32);

  // contains
  for (i32 i = 0; i < 10; ++i)
    assert(aoc_set_i32_contains(&set, i) == (i % 2 != 0));

  // remove
  for (i32 i = 0; i < 10; ++i)
    if (i % 2 != 0)
      aoc_set_i32_remove(&set, i);

  assert(set.count == 0);
  assert(set.capacity == 32);

  // test resize behavior
  for (i32 i = 0; i < 32; ++i)
    aoc_set_i32_insert(&set, i);

  assert(set.count == 32);
  assert(set.capacity == 64);

  for (i32 i = 0; i < 32; ++i)
    assert(aoc_set_i32_contains(&set, i));
  for (i32 i = 32; i < 1000; ++i)
    assert(!aoc_set_i32_contains(&set, i));

  // test iterating
  aoc_set_iter_i32 iter = {0};
  aoc_set_iter_i32_init(&iter, &set);
  i32 item = 0;
  i32 items[32] = {0};
  i32 count = 0;
  while (aoc_set_i32_iterate(&iter, &item))
    items[count++] = item;

  assert(count == set.count);
  // set is unordered. sort first and then check items
  qsort(items, count, sizeof(i32), compare_i32);
  for (i32 i = 0; i < 32; ++i)
    assert(items[i] == i);

  // duplicate
  aoc_set_i32 dup = {0};
  aoc_set_i32_duplicate(&dup, &set);

  assert(dup.capacity == set.capacity);
  assert(dup.count == set.count);
  assert(dup.entries != set.entries);

  for (i32 i = 0; i < 32; ++i)
    assert(aoc_set_i32_contains(&dup, i));
  for (i32 i = 32; i < 1000; ++i)
    assert(!aoc_set_i32_contains(&dup, i));

  // clear
  aoc_set_i32_clear(&dup);
  assert(dup.capacity == set.capacity);
  assert(dup.count == 0);

  // copy
  aoc_set_i32_copy(&dup, &set);
  assert(dup.capacity == set.capacity);
  assert(dup.count == set.count);
  assert(dup.entries != set.entries);

  for (i32 i = 0; i < 32; ++i)
    assert(aoc_set_i32_contains(&dup, i));
  for (i32 i = 32; i < 1000; ++i)
    assert(!aoc_set_i32_contains(&dup, i));

  aoc_set_i32_destroy(&dup);
  aoc_set_i32_destroy(&set);

  printf("all tests finished successfully\n");
}
