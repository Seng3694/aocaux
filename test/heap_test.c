#include <stdio.h>
#include <stdlib.h>
#include <aoc/aoc.h>

#define AOC_T i32
#define AOC_T_COMPARE(a, b) ((*(a)) - (*(b)))
#include <aoc/heap.h>

#define AOC_T i32
#define AOC_T_COMPARE(a, b) ((*(a)) - (*(b)))
#define AOC_MAX_HEAP
#include <aoc/heap.h>

int main(void) {
  const i32 items[] = {9, 3, 5, 1, 7, 0, 4, 8, 6, 2};

  aoc_min_heap_i32 minHeap = {0};
  aoc_min_heap_i32_create(&minHeap, 32);

  assert(minHeap.count == 0);
  assert(minHeap.capacity == 32);

  for (i32 i = 0; i < 10; ++i)
    aoc_min_heap_i32_push(&minHeap, items[i]);

  assert(minHeap.count == 10);
  assert(minHeap.capacity == 32);

  assert(aoc_min_heap_i32_peek(&minHeap) == 0);

  i32 item = 0;
  while (minHeap.count > 0)
    assert(item++ == aoc_min_heap_i32_pop(&minHeap));

  assert(minHeap.count == 0);
  assert(minHeap.capacity == 32);

  // resize behavior
  for (i32 i = 0; i < 32; ++i)
    aoc_min_heap_i32_push(&minHeap, i);

  assert(minHeap.count == 32);
  assert(minHeap.capacity == 32);

  aoc_min_heap_i32_push(&minHeap, 32);

  assert(minHeap.count == 33);
  assert(minHeap.capacity == 64);

  aoc_min_heap_i32_destroy(&minHeap);

  // max heap
  aoc_max_heap_i32 maxHeap = {0};
  aoc_max_heap_i32_create(&maxHeap, 32);

  for (i32 i = 0; i < 10; ++i)
    aoc_max_heap_i32_push(&maxHeap, items[i]);

  assert(aoc_max_heap_i32_peek(&maxHeap) == 9);

  item = 9;
  while (maxHeap.count > 0)
    assert(item-- == aoc_max_heap_i32_pop(&maxHeap));

  aoc_max_heap_i32_destroy(&maxHeap);

  printf("all tests finished successfully\n");
}
