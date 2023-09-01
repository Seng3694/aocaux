#include "aoc/mem.h"
#include <string.h>
void AocMemCopy(void *const dest, const void *const src,
                const AOC_SIZE_T size) {
  const uint8_t *const s = (uint8_t *)src;
  uint8_t *const d = (uint8_t *)dest;
  for (AOC_SIZE_T i = 0; i < size; ++i)
    d[i] = s[i];
}
