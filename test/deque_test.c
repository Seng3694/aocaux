#include <aoc/aoc.h>
#include <assert.h>

typedef struct {
  int32_t x;
  int32_t y;
} vec2i;

#define AOC_T vec2i
#define AOC_BASE2_CAPACITY
#include <aoc/deque.h>

int main(void) {
  aoc_deque_vec2i deque = {0};
  aoc_deque_vec2i_create(&deque, 32);

  assert(deque.capacity == 32);
  assert(deque.length == 0);

  aoc_deque_vec2i_push_back(&deque, (vec2i){1, 2});
  assert(deque.capacity == 32);
  assert(deque.length == 1);
  assert(aoc_deque_vec2i_peek_front(&deque)->x == 1);
  assert(aoc_deque_vec2i_peek_front(&deque)->y == 2);
  assert(aoc_deque_vec2i_peek_back(&deque)->x == 1);
  assert(aoc_deque_vec2i_peek_back(&deque)->y == 2);

  aoc_deque_vec2i_pop_back(&deque);
  assert(deque.capacity == 32);
  assert(deque.length == 0);
  aoc_deque_vec2i_push_front(&deque, (vec2i){1, 2});
  assert(deque.capacity == 32);
  assert(deque.length == 1);
  aoc_deque_vec2i_pop_front(&deque);
  assert(deque.capacity == 32);
  assert(deque.length == 0);

  for (int i = 0; i < 10; ++i) {
    if (i % 2 == 0)
      aoc_deque_vec2i_push_back(&deque, (vec2i){i, i});
    else
      aoc_deque_vec2i_push_front(&deque, (vec2i){i, i});
  }

  const int expectedSequence[] = {9, 7, 5, 3, 1, 0, 2, 4, 6, 8};
  aoc_deque_iter_vec2i iter = {0};
  aoc_deque_iter_vec2i_init(&iter, &deque);
  vec2i v = {0};
  int index = 0;
  while (aoc_deque_vec2i_iterate(&iter, &v))
    assert(v.x == expectedSequence[index++]);

  index = 0;
  aoc_deque_iter_vec2i_init(&iter, &deque);
  while (aoc_deque_vec2i_iterate_rev(&iter, &v)) {
    assert(v.x == expectedSequence[9 - index]);
    index++;
  }

  aoc_deque_vec2i_clear(&deque);

  for (int i = 0; i < 42; ++i)
    aoc_deque_vec2i_push_back(&deque, (vec2i){i, i});

  assert(deque.length == 42);
  assert(deque.capacity == 64);

  aoc_deque_vec2i_destroy(&deque);
}
