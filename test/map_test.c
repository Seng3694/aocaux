#include <aoc/aoc.h>
#include <stdio.h>
#include <stdlib.h>

// because map uses a set internally this is for testing whether it works when
// the set was defined before
#define AOC_T i32
#define AOC_T_EMPTY ((i32){INT32_MIN})
#define AOC_T_HASH(x) ((((u32)(*(x))) * 36591911) ^ 75656261)
#define AOC_T_EQUALS(a, b) (*(a) == *(b))
#define AOC_BASE2_CAPACITY
#include <aoc/set.h>

typedef struct {
  i32 x;
  i32 y;
} point;

#define AOC_KEY_T i32
#define AOC_VALUE_T point
#define AOC_KEY_T_EMPTY ((i32){INT32_MIN})
#define AOC_KEY_T_HASH(x) ((((u32)(*(x))) * 36591911) ^ 75656261)
#define AOC_KEY_T_EQUALS(a, b) (*(a) == *(b))
#define AOC_BASE2_CAPACITY
#define AOC_SET_NO_IMPL
#include <aoc/map.h>

typedef struct {
  i32 key;
  point value;
} kvp;

static inline int compare_kvp(const void *const a, const void *const b) {
  return ((kvp *)a)->key - ((kvp *)b)->key;
}

int main(void) {
  aoc_map_i32_point map = {0};
  aoc_map_i32_point_create(&map, 32);

  assert(map.set.count == 0);
  assert(map.set.capacity == 32);

  // insert odd numbers
  for (i32 i = 0; i < 10; ++i)
    if (i % 2 != 0)
      aoc_map_i32_point_insert(&map, i, (point){i, i});

  assert(map.set.count == 5);
  assert(map.set.capacity == 32);

  // contains
  for (i32 i = 0; i < 10; ++i)
    assert(aoc_map_i32_point_contains(&map, i) == (i % 2 != 0));

  // remove
  for (i32 i = 0; i < 10; ++i)
    if (i % 2 != 0)
      aoc_map_i32_point_remove(&map, i);

  assert(map.set.count == 0);
  assert(map.set.capacity == 32);

  // test resize behavior
  for (i32 i = 0; i < 32; ++i)
    aoc_map_i32_point_insert(&map, i, (point){i, i});

  assert(map.set.count == 32);
  assert(map.set.capacity == 64);

  for (i32 i = 0; i < 32; ++i)
    assert(aoc_map_i32_point_contains(&map, i));
  for (i32 i = 32; i < 1000; ++i)
    assert(!aoc_map_i32_point_contains(&map, i));

  // test iterating
  aoc_map_iter_i32_point iter = {0};
  aoc_map_iter_i32_point_init(&iter, &map);
  i32 key = 0;
  point value = {0};
  kvp pairs[32] = {0};
  i32 count = 0;
  while (aoc_map_i32_point_iterate(&iter, &key, &value)) {
    pairs[count].key = key;
    pairs[count++].value = value;
  }

  assert(count == map.set.count);
  // map is unordered. sort first and then check items
  qsort(pairs, count, sizeof(kvp), compare_kvp);
  for (i32 i = 0; i < 32; ++i) {
    assert(pairs[i].key == i);
    assert(pairs[i].value.x == i && pairs[i].value.y == i);
  }

  // duplicate
  aoc_map_i32_point dup = {0};
  aoc_map_i32_point_duplicate(&dup, &map);

  assert(dup.set.capacity == map.set.capacity);
  assert(dup.set.count == map.set.count);
  assert(dup.set.entries != map.set.entries);

  for (i32 i = 0; i < 32; ++i)
    assert(aoc_map_i32_point_contains(&dup, i));
  for (i32 i = 32; i < 1000; ++i)
    assert(!aoc_map_i32_point_contains(&dup, i));

  // clear
  aoc_map_i32_point_clear(&dup);
  assert(dup.set.capacity == map.set.capacity);
  assert(dup.set.count == 0);

  // put & get
  aoc_map_i32_point_put(&dup, 123, (point){1, 2});
  point item = {0};
  assert(aoc_map_i32_point_get(&dup, 123, &item));
  assert(item.x == 1 && item.y == 2);

  // put should be able to override items if they exist
  aoc_map_i32_point_put(&dup, 123, (point){7, 5});
  assert(aoc_map_i32_point_get(&dup, 123, &item));
  assert(item.x == 7 && item.y == 5);

  // get with non existent key
  assert(!aoc_map_i32_point_get(&dup, 0, &item));
  aoc_map_i32_point_remove(&map, 123);

  // copy
  aoc_map_i32_point_copy(&dup, &map);
  assert(dup.set.capacity == map.set.capacity);
  assert(dup.set.count == map.set.count);
  assert(dup.set.entries != map.set.entries);

  for (i32 i = 0; i < 32; ++i)
    assert(aoc_map_i32_point_contains(&dup, i));
  for (i32 i = 32; i < 1000; ++i)
    assert(!aoc_map_i32_point_contains(&dup, i));

  aoc_map_i32_point_destroy(&dup);
  aoc_map_i32_point_destroy(&map);

  printf("all tests finished successfully\n");
}
