#include "aoc/common.h"
#include "aoc/filesystem.h"
#include "aoc/mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool aoc_file_read_lines1(const char *path, aoc_line_func func,
                          void *userData) {
  FILE *file = fopen(path, "r");
  if (!file) {
    AOC_LOG("Could not open file '%s'.\n", path);
    return false;
  }

  char buffer[1024];
  while (fgets(buffer, 1024, file) != NULL) {
    func(buffer, userData);
  }

  fclose(file);

  return true;
}

bool aoc_file_read_lines2(const char *path, aoc_line_num_func func,
                          void *userData) {
  FILE *file = fopen(path, "r");
  if (!file) {
    AOC_LOG("Could not open file '%s'.\n", path);
    return false;
  }

  char buffer[1024];
  size_t lineNumber = 0;
  while (fgets(buffer, 1024, file) != NULL) {
    func(buffer, userData, lineNumber);
    lineNumber++;
  }

  fclose(file);

  return true;
}

bool aoc_file_read_lines3(const char *path, aoc_line_func begin,
                          aoc_line_num_func func, aoc_line_num_func end,
                          void *userData) {
  FILE *file = fopen(path, "r");
  if (!file) {
    AOC_LOG("Could not open file '%s'.\n", path);
    return false;
  }

  bool hadContent = false;
  char buffer[1024];
  size_t lineNumber = 0;
  if (fgets(buffer, 1024, file)) {
    begin(buffer, userData);
    func(buffer, userData, 0);
    hadContent = true;
  }

  while (fgets(buffer, 1024, file) != NULL) {
    func(buffer, userData, lineNumber);
    lineNumber++;
  }

  if (hadContent)
    end(buffer, userData, lineNumber);

  fclose(file);

  return true;
}

bool aoc_file_read_all1(const char *path, char **output, size_t *length) {
  FILE *file = fopen(path, "r");
  if (!file) {
    AOC_LOG("Failed to open file '%s'\n", path);
    return false;
  }
  fseek(file, 0, SEEK_END);
  const size_t size = ftell(file);
  rewind(file);

  char *content = NULL;
  // reuse array if possible
  if (*output != NULL && *length >= size) {
    content = *output;
  } else {
    content = aoc_alloc(size + 1);
    if (!content) {
      AOC_LOG("Failed to allocate %zu bytes\n", size + 1);
      fclose(file);
      return false;
    }
  }
  content[size] = '\0';
  const size_t blocksRead = fread(content, size, 1, file);
  fclose(file);
  if (blocksRead != 1) {
    return false;
  }
  *length = size;
  *output = content;
  return true;
}

char *aoc_file_read_all2(const char *path) {
  FILE *file = fopen(path, "r");
  if (!file) {
    AOC_LOG("Failed to open file '%s'\n", path);
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  const size_t size = ftell(file);
  rewind(file);

  char *content = NULL;
  // reuse array if possible
  content = aoc_alloc(size + 1);
  if (!content) {
    AOC_LOG("Failed to allocate %zu bytes\n", size + 1);
    fclose(file);
    return NULL;
  }
  content[size] = '\0';
  const size_t blocksRead = fread(content, size, 1, file);
  fclose(file);
  if (blocksRead != 1) {
    return NULL;
  }
  return content;
}
