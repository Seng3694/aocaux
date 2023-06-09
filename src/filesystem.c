#include "aoc/filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool AocReadFileLineByLine(const char *path, aoc_line_func func,
                           void *userData) {
  FILE *file = fopen(path, "r");
  if (!file) {
    AOC_LOG("Could not open file '%s'.\n", path);
    return false;
  }

  char buffer[1024];
  while (fgets(buffer, 1024, file) != NULL) {
    size_t length = strlen(buffer);
    func(buffer, length, userData);
  }

  fclose(file);

  return true;
}

bool AocReadFileLineByLineEx(const char *path, aoc_line_ex_func func,
                             void *userData) {
  FILE *file = fopen(path, "r");
  if (!file) {
    AOC_LOG("Could not open file '%s'.\n", path);
    return false;
  }

  char buffer[1024];
  size_t lineNumber = 0;
  while (fgets(buffer, 1024, file) != NULL) {
    size_t length = strlen(buffer);
    func(buffer, length, userData, lineNumber);
    lineNumber++;
  }

  fclose(file);

  return true;
}

bool AocReadFileToString(const char *path, char **output, size_t *length) {
  FILE *file = fopen(path, "r");
  if (!file) {
    AOC_LOG("Failed to open file '%s'\n", path);
    return false;
  }
  fseek(file, 0, SEEK_END);
  const size_t size = ftell(file);
  rewind(file);
  char *content = malloc(size + 1);
  if (!content) {
    AOC_LOG("Failed to allocate %zu bytes\n", size + 1);
    fclose(file);
    return false;
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
