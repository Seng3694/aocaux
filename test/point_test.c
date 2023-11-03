#include <stdlib.h>
#include <stdio.h>
#include <aoc/aoc.h>

#define AOC_T i64
#include <aoc/point.h>

int main(void) {
  const aoc_point2_i64 a = {10, 20};
  const aoc_point2_i64 b = {20, 30};

  assert(aoc_point2_i64_compare(&a, &b) < 0);
  assert(aoc_point2_i64_compare(&b, &a) > 0);
  assert(aoc_point2_i64_compare(&a, &a) == 0);

  assert(aoc_point2_i64_equals(&a, &b) == false);
  assert(aoc_point2_i64_equals(&a, &a) == true);

  assert(aoc_point2_i64_hash(&a) != aoc_point2_i64_hash(&b));
  assert(aoc_point2_i64_hash(&a) == aoc_point2_i64_hash(&a));

  assert(aoc_point2_i64_taxicab(&a, &b) == 20);
  assert(aoc_point2_i64_taxicab(&b, &a) == 20);
  assert(aoc_point2_i64_taxicab(&a, &a) == 0);

  aoc_point2_i64 points[] = {
      {10, -20}, {20, 10},   {-20, 10},  {-10, 20},
      {10, 20},  {-10, -20}, {-20, -10}, {20, -10},
  };

  for (size_t i = 0; i < AOC_ARRAY_SIZE(points) - 1; ++i) {
    for (size_t j = i + 1; j < AOC_ARRAY_SIZE(points); ++j) {
      assert(aoc_point2_i64_hash(&points[i]) !=
             aoc_point2_i64_hash(&points[j]));
    }
  }

  // expect sort in reading order
  const aoc_point2_i64 sorted[] = {
      {-10, -20}, {10, -20}, {-20, -10}, {20, -10},
      {-20, 10},  {20, 10},  {-10, 20},  {10, 20},
  };

  qsort(points, AOC_ARRAY_SIZE(points), sizeof(aoc_point2_i64),
        aoc_point2_i64_compare);

  for (size_t i = 0; i < AOC_ARRAY_SIZE(points); ++i)
    assert(aoc_point2_i64_equals(&points[i], &sorted[i]));
}
