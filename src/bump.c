#include "aoc/bump.h"
#include <stdlib.h>

#define ALIGN_TO_PTR_SIZE(size)                                                \
  (size + sizeof(uintptr_t) - 1) & (~(sizeof(uintptr_t) - 1))

void *aoc_bump_alloc(aoc_bump *const bump, AOC_SIZE_T size) {
  size = ALIGN_TO_PTR_SIZE(size);
  size = (size / sizeof(uintptr_t)) + 1; // +1 for the block size
  AOC_ASSERT(bump->length + size <= bump->capacity);
  // save block size. required for realloc
  bump->memory[bump->length] = (uintptr_t)size;
  uintptr_t *result = &bump->memory[bump->length + 1];
  bump->length += size;
#ifndef NDEBUG
  if (bump->length > bump->max)
    bump->max = bump->length;
#endif
  return result;
}

void *aoc_bump_calloc(aoc_bump *const bump, AOC_SIZE_T count, AOC_SIZE_T size) {
  size = count * size;
  uint8_t *data = aoc_bump_alloc(bump, size);
  for (size_t i = 0; i < size; ++i)
    data[i] = 0;
  return data;
}

void *aoc_bump_realloc(aoc_bump *const bump, void *old, AOC_SIZE_T size) {
  const AOC_SIZE_T oldSize = (AOC_SIZE_T)(*((uintptr_t *)old) - 1);
  if (oldSize >= size)
    return old;
  void *new = aoc_bump_alloc(bump, size);
  aoc_mem_copy(new, old, oldSize);
  return new;
}

void aoc_bump_init(aoc_bump *const bump, AOC_SIZE_T size) {
  size = ALIGN_TO_PTR_SIZE(size);
  bump->memory = malloc(size);
  AOC_ASSERT(bump->memory);
  bump->length = 0;
  bump->capacity = size / sizeof(uintptr_t);
#ifndef NDEBUG
  bump->max = 0;
#endif
}

void aoc_bump_destroy(aoc_bump *const bump) {
  free(bump->memory);
#ifndef NDEBUG
  AOC_LOG("aoc_bump memory freed\n allocated %zu\n used %zu (%.2f)\n",
          bump->capacity * sizeof(uintptr_t), bump->max * sizeof(uintptr_t),
          (double)(bump->max * sizeof(uintptr_t)) /
              (double)(bump->capacity * sizeof(uintptr_t)) * 100.0);
#endif
}

void aoc_bump_reset(aoc_bump *const bump) {
  bump->length = 0;
}

static inline void *bump_alloc(void *bump, AOC_SIZE_T size) {
  return aoc_bump_alloc(bump, size);
}

static inline void *bump_calloc(void *bump, AOC_SIZE_T count, AOC_SIZE_T size) {
  return aoc_bump_calloc(bump, count, size);
}

static inline void *bump_realloc(void *bump, void *old, AOC_SIZE_T size) {
  return aoc_bump_realloc(bump, old, size);
}

static inline void bump_free(void *bump, void *ptr) {
  (void)bump;
  (void)ptr;
}

aoc_allocator aoc_bump_create_allocator(aoc_bump *bump) {
  return (aoc_allocator){
      .alloc = bump_alloc,
      .calloc = bump_calloc,
      .realloc = bump_realloc,
      .free = bump_free,
      .allocator = bump,
  };
}
