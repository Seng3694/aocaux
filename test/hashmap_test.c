#include <aoc/aoc.h>
#include <stdio.h>

typedef struct {
  size_t length;
  size_t capacity;
  char *data;
} AocString;

typedef struct {
  float x;
  float y;
} position;

static uint32_t calc_string_hash(const AocString *const str) {
  uint32_t hash = 36591911;
  const char *s = str->data;
  while (*s) {
    hash *= (*s) ^ 17499491;
    s++;
  }
  return hash;
}

static bool string_equals(const AocString *const a, const AocString *const b) {
  if (a->data == NULL && b->data == NULL)
    return true;
  else if (a->data == NULL || b->data == NULL)
    return false;
  else if (a->length != b->length)
    return false;
  return calc_string_hash(a) == calc_string_hash(b);
}

static const AocString EMPTY_STRING = {0};

#define AOC_KEY_T AocString
#define AOC_KEY_T_NAME String
#define AOC_VALUE_T position
#define AOC_VALUE_T_NAME Pos
#define AOC_KEY_T_EMPTY EMPTY_STRING
#define AOC_KEY_T_HFUNC calc_string_hash
#define AOC_KEY_T_EQUALS string_equals
#define AOC_BASE2_CAPACITY
#include <aoc/hashmap.h>

int main(void) {
  AocHashmapStringPos hm;
  AocHashmapStringPosCreate(&hm, 32);

  AocString strings[] = {
      (AocString){
          .data = "hello world",
          .capacity = 12,
          .length = 11,
      },
      (AocString){
          .data = "hi mom",
          .capacity = 7,
          .length = 6,
      },
      (AocString){
          .data = "woot",
          .capacity = 5,
          .length = 4,
      },
  };

  AocHashmapStringPosInsert(&hm, strings[0], (position){.x = 10, .y = 12});
  AocHashmapStringPosInsert(&hm, strings[1], (position){.x = 43, .y = 63});

  position pos;
  if (AocHashmapStringPosGet(&hm, strings[0], &pos)) {
    printf("key found! value: (%.2f|%.2f)\n", pos.x, pos.y);
  } else {
    printf("key not found\n");
  }

  AocHashmapStringPosRemove(&hm, strings[1]);

  if (AocHashmapStringPosGet(&hm, strings[1], &pos)) {
    printf("key found! value: (%.2f|%.2f)\n", pos.x, pos.y);
  } else {
    printf("key not found\n");
  }

  if (AocHashmapStringPosGet(&hm, strings[2], &pos)) {
    printf("key found! value: (%.2f|%.2f)\n", pos.x, pos.y);
  } else {
    printf("key not found\n");
  }

  AocHashmapStringPosDestroy(&hm);
}