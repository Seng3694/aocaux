#ifndef AOCAUX_IMAGE_H
#define AOCAUX_IMAGE_H

#include "common.h"

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} aoc_color;

void aoc_image_clear(const aoc_color clearColor, const AOC_SIZE_T pixelCount,
                     aoc_color *const pixelData);

bool aoc_image_create(const char *filePath, const AOC_SIZE_T width,
                      const AOC_SIZE_T height,
                      const aoc_color *const pixelData);

#endif
