#ifndef AOCAUX_FILESYSTEM_H
#define AOCAUX_FILESYSTEM_H

#include "common.h"

typedef void (*aoc_line_func)(char *line, void *userData);
typedef void (*aoc_line_num_func)(char *line, void *userData,
                                  const size_t lineNumber);

bool aoc_file_read_lines1(const char *path, aoc_line_func func, void *userData);
bool aoc_file_read_lines2(const char *path, aoc_line_num_func func,
                          void *userData);
bool aoc_file_read_lines3(const char *path, aoc_line_func begin,
                          aoc_line_num_func func, aoc_line_num_func end,
                          void *userData);

bool aoc_file_read_all1(const char *path, char **output, size_t *length);
char *aoc_file_read_all2(const char *path);

#endif
