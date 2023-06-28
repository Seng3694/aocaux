#include <aoc/aoc.h>
#include <assert.h>

typedef struct {
  int32_t x;
  int32_t y;
} vec2i;

#define AOC_T vec2i
#define AOC_T_NAME Vec2i
#define AOC_BASE2_CAPACITY
#include <aoc/deque.h>

int main(void) {
  AocDequeVec2i deque = {0};
  AocDequeVec2iCreate(&deque, 32);

  assert(deque.capacity == 32);
  assert(deque.length == 0);

  AocDequeVec2iPushBack(&deque, (vec2i){1, 2});
  assert(deque.capacity == 32);
  assert(deque.length == 1);
  assert(AocDequeVec2iPeekFront(&deque)->x == 1);
  assert(AocDequeVec2iPeekFront(&deque)->y == 2);
  assert(AocDequeVec2iPeekBack(&deque)->x == 1);
  assert(AocDequeVec2iPeekBack(&deque)->y == 2);

  AocDequeVec2iPopBack(&deque);
  assert(deque.capacity == 32);
  assert(deque.length == 0);
  AocDequeVec2iPushFront(&deque, (vec2i){1, 2});
  assert(deque.capacity == 32);
  assert(deque.length == 1);
  AocDequeVec2iPopFront(&deque);
  assert(deque.capacity == 32);
  assert(deque.length == 0);

  for (int i = 0; i < 10; ++i) {
    if (i % 2 == 0)
      AocDequeVec2iPushBack(&deque, (vec2i){i, i});
    else
      AocDequeVec2iPushFront(&deque, (vec2i){i, i});
  }

  const int expectedSequence[] = {9, 7, 5, 3, 1, 0, 2, 4, 6, 8};
  int index = 0;
  while (deque.length > 0) {
    const int value = AocDequeVec2iPeekFront(&deque)->x;
    AocDequeVec2iPopFront(&deque);
    assert(value == expectedSequence[index++]);
  }

  for (int i = 0; i < 42; ++i)
    AocDequeVec2iPushBack(&deque, (vec2i){i, i});

  assert(deque.length == 42);
  assert(deque.capacity == 64);

  AocDequeVec2iDestroy(&deque);
}
