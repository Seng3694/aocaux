#ifndef AOCAUX_MD5_H
#define AOCAUX_MD5_H

#include "common.h"

void AocMD5(const char *text, const size_t length, uint8_t *const result);

void AocMD5ResultToString(const uint8_t *const result, char *const buffer);

#endif
