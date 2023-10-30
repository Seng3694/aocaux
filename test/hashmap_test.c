#include <aoc/aoc.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  size_t length;
  size_t capacity;
  char *data;
} string;

static inline string create_string(char *str) {
  return (string){
      .data = str,
      .capacity = strlen(str) + 1,
      .length = strlen(str),
  };
}

typedef struct {
  size_t x;
  size_t y;
} position;

static u32 calc_string_hash(const string *const str) {
  u32 hash = 36591911;
  const char *s = str->data;
  while (*s) {
    hash = ((hash << 5) + hash) + *s;
    s++;
  }
  return hash;
}

static bool string_equals(const string *const a, const string *const b) {
  if (a->data == NULL && b->data == NULL)
    return true;
  else if (a->data == NULL || b->data == NULL)
    return false;
  else if (a->length != b->length)
    return false;
  return calc_string_hash(a) == calc_string_hash(b);
}

static const string EMPTY_STRING = {0};

#define AOC_KEY_T string
#define AOC_KEY_T_NAME str
#define AOC_VALUE_T position
#define AOC_VALUE_T_NAME pos
#define AOC_KEY_T_EMPTY EMPTY_STRING
#define AOC_KEY_T_HFUNC calc_string_hash
#define AOC_KEY_T_EQUALS string_equals
#define AOC_BASE2_CAPACITY
#include <aoc/hashmap.h>

int main(void) {
  aoc_map_str_pos map;
  aoc_map_str_pos_create(&map, 32);

  assert(map.capacity == 32);
  assert(map.count == 0);

  string strings[] = {
      create_string("hello world"),
      create_string("hi mom"),
      create_string("woot"),
  };

  aoc_map_str_pos_insert(&map, strings[0], (position){.x = 10, .y = 12});
  aoc_map_str_pos_insert(&map, strings[1], (position){.x = 43, .y = 63});
  aoc_map_str_pos_insert(&map, strings[2], (position){.x = 123, .y = 45});

  assert(map.capacity == 32);
  assert(map.count == 3);

  position pos;
  bool found = aoc_map_str_pos_get(&map, strings[0], &pos);
  assert(found);
  assert(pos.x == 10 && pos.y == 12);

  found = aoc_map_str_pos_get(&map, strings[1], &pos);
  assert(found);
  assert(pos.x == 43 && pos.y == 63);

  found = aoc_map_str_pos_get(&map, strings[2], &pos);
  assert(found);
  assert(pos.x == 123 && pos.y == 45);

  aoc_map_str_pos_remove(&map, strings[1]);

  assert(map.capacity == 32);
  assert(map.count == 2);

  found = aoc_map_str_pos_get(&map, strings[1], &pos);
  assert(!found);

  aoc_map_iter_str_pos iter = {0};
  aoc_map_iter_str_pos_init(&iter, &map);

  string key = {0};
  position value = {0};
  int iterations = 0;
  while (aoc_map_str_pos_iterate(&iter, &key, &value)) {
    iterations++;
    // unordered so check both
    assert(
        (string_equals(&key, &strings[0]) && value.x == 10 && value.y == 12) ||
        (string_equals(&key, &strings[2]) && value.x == 123 && value.y == 45));
  }
  assert(iterations == 2);

  // this is fine because the strings used were not allocated
  aoc_map_str_pos_clear(&map);

  assert(map.capacity == 32);
  assert(map.count == 0);

  // check resize behavior
  // currently 0.75 is the hard coded threshold until it's getting resized
  // but this might change in the future so let's just add 32 items
  // it should always resize by x2 to not break the base 2 compatibility though
  char buffer[3] = {0};
  for (size_t i = 0; i < 32; ++i) {
    snprintf(buffer, 3, "%zu", i);
    char *str = aoc_alloc(3);
    aoc_mem_copy(str, buffer, 3);
    string s = create_string(str);
    position p = {i, 31 - i};
    aoc_map_str_pos_insert(&map, s, p);
  }

  assert(map.capacity == 64);
  assert(map.count == 32);

  // try finding all items
  for (size_t i = 0; i < 32; ++i) {
    snprintf(buffer, 3, "%zu", i);
    position p = {0};
    found = aoc_map_str_pos_get(&map, create_string(buffer), &p);
    assert(found);
    assert(p.x == i);
    assert(p.y == (31 - i));
  }

  // iterate and free all keys
  iterations = 0;
  aoc_map_iter_str_pos_init(&iter, &map);
  while (aoc_map_str_pos_iterate(&iter, &key, &value)) {
    iterations++;
    aoc_free(key.data);
  }
  assert(iterations == 32);
  aoc_map_str_pos_clear(&map);
  aoc_map_str_pos_destroy(&map);
}
