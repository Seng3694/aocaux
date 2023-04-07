#include "aoc/string.h"
#include "aoc/common.h"
#include <ctype.h>

void AocTrimRight(char *const str, size_t *const length) {
  if (*length == 0)
    return;
  char *current = &str[*length - 1];
  while (current >= str && isspace(*current)) {
    *current = '\0';
    current--;
    (*length)--;
  }
}
