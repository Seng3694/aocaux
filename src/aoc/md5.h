#ifndef AOCAUX_MD5_H
#define AOCAUX_MD5_H

#include "common.h"

void aoc_md5(const char *text, const AOC_SIZE_T length, u8 *const result);

void aoc_md5_string(const u8 *const result, char *const buffer);

#endif
