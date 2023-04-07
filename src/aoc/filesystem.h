#ifndef AOCAUX_FILESYSTEM_H
#define AOCAUX_FILESYSTEM_H

#include "common.h"

typedef void (*aoc_line_func)(char *line, size_t length, void *userData);

typedef void (*aoc_line_ex_func)(char *line, size_t length, void *userData,
                                 const size_t lineNumber);

bool AocReadFileLineByLine(const char *path, aoc_line_func func,
                           void *userData);
bool AocReadFileLineByLineEx(const char *path, aoc_line_ex_func func,
                             void *userData);
bool AocReadFileToString(const char *path, char **output, size_t *length);

#endif
