#include "aoc/image.h"
#include "aoc/mem.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_ASSERT AOC_ASSERT
#define STBIW_MALLOC aoc_alloc
#define STBIW_REALLOC aoc_realloc
#define STBIW_FREE aoc_free
#include "../ext/stb_image_write.h"

void aoc_image_clear(const aoc_color clearColor, const AOC_SIZE_T pixelCount,
                     aoc_color *const pixelData) {
  for (AOC_SIZE_T i = 0; i < pixelCount; ++i)
    pixelData[i] = clearColor;
}

bool aoc_image_create(const char *fileName, const AOC_SIZE_T width,
                      const AOC_SIZE_T height,
                      const aoc_color *const pixelData) {
  return stbi_write_png(fileName, width, height, 3, pixelData, width * 3) == 0;
}
