#include "common.h"
#include "mem.h"

// these macros have to be defined
// #define AOC_KEY_T
// #define AOC_VALUE_T
// #define AOC_KEY_T_EMPTY
// #define AOC_KEY_T_HASH(x)
// #define AOC_KEY_T_EQUALS(a, b)

// these macros are optional
// #define AOC_BASE2_CAPACITY
// #define AOC_KEY_T_NAME
// #define AOC_VALUE_T_NAME
// #define AOC_MAP_NO_IMPL
// #define AOC_SET_NO_IMPL

#ifndef AOC_KEY_T
#error "AOC_KEY_T must be defined"
#endif

#ifndef AOC_KEY_T_NAME
#define AOC_KEY_T_NAME AOC_KEY_T
#endif

#ifndef AOC_VALUE_T
#error "AOC_VALUE_T must be defined"
#endif

#ifndef AOC_VALUE_T_NAME
#define AOC_VALUE_T_NAME AOC_VALUE_T
#endif

#ifndef AOC_KEY_T_EMPTY
#error "AOC_KEY_T_EMPTY must be defined"
#endif

#ifndef AOC_KEY_T_HASH
#error "AOC_KEY_T_HASH must be defined"
#endif

#ifndef AOC_KEY_T_EQUALS
#error "AOC_KEY_T_EQUALS must be defined"
#endif

#ifdef AOC_BASE2_CAPACITY
#define MAP_MOD(a, b) ((a) & ((b)-1))
#else
#define MAP_MOD(a, b) ((a) % (b))
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define AOC_INTERNAL_INCLUDE
#define AOC_T AOC_KEY_T
#define AOC_T_NAME AOC_KEY_T_NAME
#define AOC_T_EMPTY AOC_KEY_T_EMPTY
#define AOC_T_HASH AOC_KEY_T_HASH
#define AOC_T_EQUALS AOC_KEY_T_EQUALS
#include "set.h"

#define MAP_NAME                                                               \
  COMBINE2(aoc_map_, COMBINE2(AOC_KEY_T_NAME, COMBINE2(_, AOC_VALUE_T_NAME)))
#define MAP_IMPL(word) COMBINE2(COMBINE2(MAP_NAME, _), word)
#define MAP_IMPL_INTERNAL(word) COMBINE2(_, MAP_IMPL(word))
#define MAP_LINKAGE static inline
#define MAP_MAX_LOAD 0.75

#define MAP_ITER_NAME                                                          \
  COMBINE2(aoc_map_iter_,                                                      \
           COMBINE2(AOC_KEY_T_NAME, COMBINE2(_, AOC_VALUE_T_NAME)))
#define MAP_ITER_IMPL(word) COMBINE2(MAP_ITER_NAME, COMBINE2(_, word))

typedef struct MAP_NAME MAP_NAME;
typedef struct MAP_ITER_NAME MAP_ITER_NAME;

#define MAP_ITER_INIT MAP_ITER_IMPL(init)
#define MAP_ITERATE MAP_IMPL(iterate)

#define MAP_CREATE MAP_IMPL(create)
#define MAP_DESTROY MAP_IMPL(destroy)

#define MAP_ADJUST_CAP MAP_IMPL_INTERNAL(adjust_capacity)
#define MAP_ENSURE_CAP MAP_IMPL_INTERNAL(ensure_capacity)

#define MAP_INSERT MAP_IMPL(insert)
#define MAP_REMOVE MAP_IMPL(remove)
#define MAP_GET MAP_IMPL(get)
#define MAP_PUT MAP_IMPL(put)
#define MAP_CONTAINS MAP_IMPL(contains)

#define MAP_INSERT_PH MAP_IMPL(insert_pre_hashed)
#define MAP_REMOVE_PH MAP_IMPL(remove_pre_hashed)
#define MAP_GET_PH MAP_IMPL(get_pre_hashed)
#define MAP_PUT_PH MAP_IMPL(put_pre_hashed)

#define MAP_CONTAINS_PH MAP_IMPL(contains_pre_hashed)

#define MAP_CLEAR MAP_IMPL(clear)
#define MAP_COPY MAP_IMPL(copy)
#define MAP_DUPLICATE MAP_IMPL(duplicate)

MAP_LINKAGE void MAP_ITER_INIT(MAP_ITER_NAME *const iter, MAP_NAME *map);
MAP_LINKAGE bool MAP_ITERATE(MAP_ITER_NAME *const iter, AOC_KEY_T *const key,
                             AOC_VALUE_T *const value);

MAP_LINKAGE void MAP_CREATE(MAP_NAME *const map, const AOC_SIZE_T capacity);
MAP_LINKAGE void MAP_DESTROY(MAP_NAME *map);
MAP_LINKAGE void MAP_ADJUST_CAP(MAP_NAME *map, const AOC_SIZE_T capacity);
MAP_LINKAGE void MAP_ENSURE_CAP(MAP_NAME *map);

MAP_LINKAGE void MAP_INSERT(MAP_NAME *const map, const AOC_KEY_T key,
                            const AOC_VALUE_T value);
MAP_LINKAGE void MAP_REMOVE(MAP_NAME *const map, const AOC_KEY_T key);
MAP_LINKAGE bool MAP_GET(MAP_NAME *const map, const AOC_KEY_T key,
                         AOC_VALUE_T *const value);
MAP_LINKAGE void MAP_PUT(MAP_NAME *const map, const AOC_KEY_T key,
                         const AOC_VALUE_T value);
MAP_LINKAGE bool MAP_CONTAINS(const MAP_NAME *const map, const AOC_KEY_T key);

MAP_LINKAGE void MAP_INSERT_PH(MAP_NAME *const map, const AOC_KEY_T key,
                               const AOC_VALUE_T value, const u32 hash);
MAP_LINKAGE void MAP_REMOVE_PH(MAP_NAME *const map, const AOC_KEY_T key,
                               const u32 hash);
MAP_LINKAGE bool MAP_GET_PH(MAP_NAME *const map, const AOC_KEY_T key,
                            AOC_VALUE_T *const value, const u32 hash);
MAP_LINKAGE void MAP_PUT_PH(MAP_NAME *const map, const AOC_KEY_T key,
                            const AOC_VALUE_T value, const u32 hash);
MAP_LINKAGE bool MAP_CONTAINS_PH(const MAP_NAME *const map, const AOC_KEY_T key,
                                 const u32 hash);

MAP_LINKAGE void MAP_CLEAR(MAP_NAME *const map);
MAP_LINKAGE void MAP_COPY(MAP_NAME *const dest, const MAP_NAME *const src);
MAP_LINKAGE void MAP_DUPLICATE(MAP_NAME *const dest, const MAP_NAME *const src);

#ifndef AOC_NO_IMPL

struct MAP_NAME {
  SET_NAME set;
  AOC_VALUE_T *values;
};

struct MAP_ITER_NAME {
  const MAP_NAME *map;
  AOC_SIZE_T current;
};

MAP_LINKAGE void MAP_CREATE(MAP_NAME *const map, const AOC_SIZE_T capacity) {
  SET_CREATE(&map->set, capacity);
  map->values = (AOC_VALUE_T *)aoc_alloc(sizeof(AOC_VALUE_T) * capacity);
  SET_CLEAR(&map->set);
}

MAP_LINKAGE void MAP_DESTROY(MAP_NAME *map) {
  SET_DESTROY(&map->set);
  aoc_free(map->values);
}

// TODO: similar to copy. maybe write a more general function
MAP_LINKAGE void MAP_ADJUST_CAP(MAP_NAME *map, const AOC_SIZE_T capacity) {
  AOC_KEY_T *keys = (AOC_KEY_T *)aoc_alloc(sizeof(AOC_KEY_T) * capacity);
  AOC_VALUE_T *values =
      (AOC_VALUE_T *)aoc_alloc(sizeof(AOC_VALUE_T) * capacity);
  for (AOC_SIZE_T i = 0; i < capacity; ++i)
    keys[i] = AOC_KEY_T_EMPTY;

  u32 hash = 0;
  AOC_SIZE_T index = 0;

  MAP_ITER_NAME iter = {.map = map};
  AOC_KEY_T key;
  AOC_VALUE_T value;
  while (MAP_ITERATE(&iter, &key, &value)) {
    hash = AOC_KEY_T_HASH(&key);
    index = SET_FIND_EMPTY(keys, capacity, hash);
    keys[index] = key;
    values[index] = value;
  }

  aoc_free(map->set.entries);
  aoc_free(map->values);
  map->set.entries = keys;
  map->values = values;
  map->set.capacity = capacity;
}

MAP_LINKAGE void MAP_ENSURE_CAP(MAP_NAME *map) {
  if (map->set.count + 1 > map->set.capacity * MAP_MAX_LOAD) {
    const AOC_SIZE_T capacity = map->set.capacity * 2;
    MAP_ADJUST_CAP(map, capacity);
  }
}

MAP_LINKAGE void MAP_INSERT(MAP_NAME *const map, const AOC_KEY_T key,
                            const AOC_VALUE_T value) {
  MAP_INSERT_PH(map, key, value, AOC_KEY_T_HASH(&key));
}

MAP_LINKAGE void MAP_INSERT_PH(MAP_NAME *const map, const AOC_KEY_T key,
                               const AOC_VALUE_T value, const u32 hash) {
  MAP_ENSURE_CAP(map);
  const AOC_SIZE_T index =
      SET_FIND_EMPTY(map->set.entries, map->set.capacity, hash);
  map->set.entries[index] = key;
  map->values[index] = value;
  map->set.count++;
}

MAP_LINKAGE void MAP_REMOVE(MAP_NAME *const map, const AOC_KEY_T key) {
  MAP_REMOVE_PH(map, key, AOC_KEY_T_HASH(&key));
}

MAP_LINKAGE void MAP_REMOVE_PH(MAP_NAME *const map, const AOC_KEY_T key,
                               const u32 hash) {
  SET_REMOVE_PH(&map->set, key, hash);
}

MAP_LINKAGE bool MAP_GET(MAP_NAME *const map, const AOC_KEY_T key,
                         AOC_VALUE_T *const value) {
  return MAP_GET_PH(map, key, value, AOC_KEY_T_HASH(&key));
}

MAP_LINKAGE bool MAP_GET_PH(MAP_NAME *const map, const AOC_KEY_T key,
                            AOC_VALUE_T *const value, const u32 hash) {
  AOC_SIZE_T index = 0;
  if (SET_FIND_ENTRY(map->set.entries, map->set.capacity, key, hash, &index)) {
    *value = map->values[index];
    return true;
  }
  return false;
}

MAP_LINKAGE void MAP_PUT(MAP_NAME *const map, const AOC_KEY_T key,
                         const AOC_VALUE_T value) {
  MAP_PUT_PH(map, key, value, AOC_KEY_T_HASH(&key));
}

MAP_LINKAGE void MAP_PUT_PH(MAP_NAME *const map, const AOC_KEY_T key,
                            const AOC_VALUE_T value, const u32 hash) {
  AOC_SIZE_T index = 0;
  if (SET_FIND_ENTRY(map->set.entries, map->set.capacity, key, hash, &index)) {
    map->values[index] = value;
  } else {
    MAP_INSERT_PH(map, key, value, hash);
  }
}

MAP_LINKAGE bool MAP_CONTAINS(const MAP_NAME *const map, const AOC_KEY_T key) {
  return MAP_CONTAINS_PH(map, key, AOC_KEY_T_HASH(&key));
}

MAP_LINKAGE bool MAP_CONTAINS_PH(const MAP_NAME *const map, const AOC_KEY_T key,
                                 const u32 hash) {
  return SET_CONTAINS_PH(&map->set, key, hash);
}

MAP_LINKAGE void MAP_CLEAR(MAP_NAME *const map) {
  SET_CLEAR(&map->set);
}

MAP_LINKAGE void MAP_COPY(MAP_NAME *const dest, const MAP_NAME *const src) {
  AOC_ASSERT(dest->set.capacity >= src->set.capacity);
  if (dest->set.count > 0)
    MAP_CLEAR(dest);

  u32 hash = 0;
  AOC_SIZE_T index = 0;

  MAP_ITER_NAME iter = {.map = src};
  AOC_KEY_T key;
  AOC_VALUE_T value;
  while (MAP_ITERATE(&iter, &key, &value)) {
    hash = AOC_KEY_T_HASH(&key);
    index = SET_FIND_EMPTY(dest->set.entries, dest->set.capacity, hash);
    dest->set.entries[index] = key;
    dest->values[index] = value;
  }
  dest->set.count = src->set.count;
}

MAP_LINKAGE void MAP_DUPLICATE(MAP_NAME *const dest,
                               const MAP_NAME *const src) {
  MAP_CREATE(dest, src->set.capacity);
  MAP_COPY(dest, src);
}

MAP_LINKAGE void MAP_ITER_INIT(MAP_ITER_NAME *const iter, MAP_NAME *map) {
  iter->map = map;
  iter->current = 0;
}

MAP_LINKAGE bool MAP_ITERATE(MAP_ITER_NAME *const iter, AOC_KEY_T *const key,
                             AOC_VALUE_T *const value) {
  if (iter->map->set.count == 0)
    return false;

  while (iter->current < iter->map->set.capacity &&
         AOC_KEY_T_EQUALS(&iter->map->set.entries[iter->current],
                          &AOC_KEY_T_EMPTY)) {
    iter->current++;
  }

  if (iter->current == iter->map->set.capacity)
    return false;

  *key = iter->map->set.entries[iter->current];
  *value = iter->map->values[iter->current];
  iter->current++;
  return true;
}

#endif

#undef AOC_KEY_T
#undef AOC_KEY_T_NAME
#undef AOC_VALUE_T
#undef AOC_VALUE_T_NAME
#undef AOC_KEY_T_EMPTY
#undef AOC_KEY_T_HASH
#undef AOC_KEY_T_EQUALS
#undef MAP_MOD
#undef COMBINE
#undef COMBINE2

#ifdef AOC_BASE2_CAPACITY
#undef AOC_BASE2_CAPACITY
#endif

#ifdef AOC_MAP_NO_IMPL
#undef AOC_MAP_NO_IMPL
#endif

#undef MAP_NAME
#undef MAP_IMPL
#undef MAP_IMPL_INTERNAL
#undef MAP_LINKAGE
#undef MAP_MAX_LOAD
#undef MAP_ITER_NAME
#undef MAP_ITER_IMPL
#undef MAP_ITER_INIT
#undef MAP_ITERATE
#undef MAP_CREATE
#undef MAP_DESTROY
#undef MAP_ADJUST_CAP
#undef MAP_ENSURE_CAP
#undef MAP_INSERT
#undef MAP_REMOVE
#undef MAP_GET
#undef MAP_PUT
#undef MAP_CONTAINS
#undef MAP_INSERT_PH
#undef MAP_REMOVE_PH
#undef MAP_GET_PH
#undef MAP_PUT_PH
#undef MAP_CONTAINS_PH
#undef MAP_CLEAR
#undef MAP_COPY
#undef MAP_DUPLICATE

// set internal defines
#undef AOC_INTERNAL_INCLUDE
#ifdef AOC_SET_NO_IMPL
#undef AOC_SET_NO_IMPL
#endif
#undef AOC_T
#undef AOC_T_NAME
#undef AOC_T_EMPTY
#undef AOC_T_HASH
#undef AOC_T_EQUALS
#undef SET_NAME
#undef SET_CREATE
#undef SET_DESTROY
#undef SET_MOD
#undef SET_IMPL
#undef SET_IMPL_INTERNAL
#undef SET_LINKAGE
#undef SET_MAX_LOAD
#undef SET_ITER_NAME
#undef SET_ITER_IMPL
#undef SET_ITER_INIT
#undef SET_ITERATE
#undef SET_ADJUST_CAP
#undef SET_FIND_ENTRY
#undef SET_FIND_EMPTY
#undef SET_ENSURE_CAP
#undef SET_INSERT
#undef SET_REMOVE
#undef SET_CONTAINS
#undef SET_INSERT_PH
#undef SET_REMOVE_PH
#undef SET_CONTAINS_PH
#undef SET_CLEAR
#undef SET_COPY
#undef SET_DUPLICATE
