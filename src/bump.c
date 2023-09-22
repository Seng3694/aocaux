#include "aoc/bump.h"
#include <stdlib.h>

#define ALIGN_TO_PTR_SIZE(size)                                                \
  (size + sizeof(uintptr_t) - 1) & (~(sizeof(uintptr_t) - 1))

void *AocBumpAlloc(aoc_bump *const bump, AOC_SIZE_T size) {
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

void *AocBumpCalloc(aoc_bump *const bump, AOC_SIZE_T count, AOC_SIZE_T size) {
  size = count * size;
  uint8_t *data = AocBumpAlloc(bump, size);
  for (size_t i = 0; i < size; ++i)
    data[i] = 0;
  return data;
}

void *AocBumpRealloc(aoc_bump *const bump, void *old, AOC_SIZE_T size) {
  const AOC_SIZE_T oldSize = (AOC_SIZE_T)(*((uintptr_t *)old) - 1);
  if (oldSize >= size)
    return old;
  void *new = AocBumpAlloc(bump, size);
  AocMemCopy(new, old, oldSize);
  return new;
}

void AocBumpInit(aoc_bump *const bump, AOC_SIZE_T size) {
  size = ALIGN_TO_PTR_SIZE(size);
  bump->memory = malloc(size);
  AOC_ASSERT(bump->memory);
  bump->length = 0;
  bump->capacity = size / sizeof(uintptr_t);
#ifndef NDEBUG
  bump->max = 0;
#endif
}

void AocBumpDestroy(aoc_bump *const bump) {
  free(bump->memory);
#ifndef NDEBUG
  AOC_LOG("aoc_bump memory freed\n allocated %zu\n used %zu (%.2f)\n",
          bump->capacity * sizeof(uintptr_t), bump->max * sizeof(uintptr_t),
          (double)(bump->max * sizeof(uintptr_t)) /
              (double)(bump->capacity * sizeof(uintptr_t)) * 100.0);
#endif
}

void AocBumpReset(aoc_bump *const bump) {
  bump->length = 0;
}

static inline void *aoc_bump_alloc(void *bump, AOC_SIZE_T size) {
  return AocBumpAlloc(bump, size);
}

static inline void *aoc_bump_calloc(void *bump, AOC_SIZE_T count,
                                    AOC_SIZE_T size) {
  return AocBumpCalloc(bump, count, size);
}

static inline void *aoc_bump_realloc(void *bump, void *old, AOC_SIZE_T size) {
  return AocBumpRealloc(bump, old, size);
}

static inline void aoc_bump_free(void *bump, void *ptr) {
  (void)bump;
  (void)ptr;
}

aoc_allocator AocBumpCreateAllocator(aoc_bump *bump) {
  return (aoc_allocator){
      .alloc = aoc_bump_alloc,
      .calloc = aoc_bump_calloc,
      .realloc = aoc_bump_realloc,
      .free = aoc_bump_free,
      .allocator = bump,
  };
}
