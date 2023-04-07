#ifndef AOCAUX_FILESYSTEM_H
#define AOCAUX_FILESYSTEM_H

#include "common.h"

typedef void (*aoc_line_func)(char *line, size_t length, void *userData);

bool AocReadFileLineByLine(const char *path, aoc_line_func func,
                           void *userData);
bool AocReadFileToString(const char *path, char **output, size_t *length);

#endif
