#include "common.h"
#include "mem.h"

// these macros have to be defined
// #define AOC_T
// #define AOC_T_EMPTY
// #define AOC_T_HASH(x)
// #define AOC_T_EQUALS(a, b)

// these macros are optional
// #define AOC_BASE2_CAPACITY
// #define AOC_T_NAME
// #define AOC_SET_NO_IMPL

#ifndef AOC_T
#error "AOC_T must be defined"
#endif

#ifndef AOC_T_NAME
#define AOC_T_NAME AOC_T
#endif

#ifndef AOC_T_EMPTY
#error "AOC_T_EMPTY must be defined"
#endif

#ifndef AOC_T_HASH
#error "AOC_T_HASH must be defined"
#endif

#ifndef AOC_T_EQUALS
#error "AOC_T_EQUALS must be defined"
#endif

#ifdef AOC_BASE2_CAPACITY
#define SET_MOD(a, b) ((a) & ((b)-1))
#else
#define SET_MOD(a, b) ((a) % (b))
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define SET_NAME COMBINE2(aoc_set_, AOC_T_NAME)
#define SET_IMPL(word) COMBINE2(COMBINE2(SET_NAME, _), word)
#define SET_IMPL_INTERNAL(word) COMBINE2(_, SET_IMPL(word))
#define SET_LINKAGE static inline
#define SET_MAX_LOAD 0.75

#define SET_ITER_NAME COMBINE2(aoc_set_iter_, AOC_T_NAME)
#define SET_ITER_IMPL(word) COMBINE2(SET_ITER_NAME, COMBINE2(_, word))

typedef struct SET_NAME SET_NAME;
typedef struct SET_ITER_NAME SET_ITER_NAME;

#define SET_ITER_INIT SET_ITER_IMPL(init)
#define SET_ITERATE SET_IMPL(iterate)

#define SET_CREATE SET_IMPL(create)
#define SET_DESTROY SET_IMPL(destroy)

#define SET_ADJUST_CAP SET_IMPL_INTERNAL(adjust_capacity)
#define SET_FIND_ENTRY SET_IMPL_INTERNAL(find_entry)
#define SET_FIND_EMPTY SET_IMPL_INTERNAL(find_empty)
#define SET_ENSURE_CAP SET_IMPL_INTERNAL(ensure_capacity)

#define SET_INSERT SET_IMPL(insert)
#define SET_REMOVE SET_IMPL(remove)
#define SET_CONTAINS SET_IMPL(contains)

#define SET_INSERT_PH SET_IMPL(insert_pre_hashed)
#define SET_REMOVE_PH SET_IMPL(remove_pre_hashed)
#define SET_CONTAINS_PH SET_IMPL(contains_pre_hashed)

#define SET_CLEAR SET_IMPL(clear)
#define SET_COPY SET_IMPL(copy)
#define SET_DUPLICATE SET_IMPL(duplicate)

SET_LINKAGE void SET_ITER_INIT(SET_ITER_NAME *const iter, SET_NAME *set);
SET_LINKAGE bool SET_ITERATE(SET_ITER_NAME *const iter, AOC_T *const key);

SET_LINKAGE void SET_CREATE(SET_NAME *const set, const AOC_SIZE_T capacity);
SET_LINKAGE void SET_DESTROY(SET_NAME *set);
SET_LINKAGE void SET_ADJUST_CAP(SET_NAME *set, const AOC_SIZE_T capacity);
SET_LINKAGE void SET_ENSURE_CAP(SET_NAME *set);

SET_LINKAGE bool SET_FIND_ENTRY(AOC_T *keys, const AOC_SIZE_T capacity,
                                const AOC_T key, const u32 hash,
                                AOC_SIZE_T *const outIndex);

SET_LINKAGE AOC_SIZE_T SET_FIND_EMPTY(AOC_T *keys, const AOC_SIZE_T capacity,
                                      const u32 hash);

SET_LINKAGE void SET_INSERT(SET_NAME *const set, const AOC_T key);
SET_LINKAGE void SET_REMOVE(SET_NAME *const set, const AOC_T key);
SET_LINKAGE bool SET_CONTAINS(const SET_NAME *const set, const AOC_T key);

SET_LINKAGE void SET_INSERT_PH(SET_NAME *const set, const AOC_T key,
                               const u32 hash);
SET_LINKAGE void SET_REMOVE_PH(SET_NAME *const set, const AOC_T key,
                               const u32 hash);
SET_LINKAGE bool SET_CONTAINS_PH(const SET_NAME *const set, const AOC_T key,
                                 const u32 hash);

SET_LINKAGE void SET_CLEAR(SET_NAME *const set);
SET_LINKAGE void SET_COPY(SET_NAME *const dest, const SET_NAME *const src);
SET_LINKAGE void SET_DUPLICATE(SET_NAME *const dest, const SET_NAME *const src);

#ifndef AOC_SET_NO_IMPL
typedef struct SET_NAME {
  AOC_SIZE_T capacity;
  AOC_SIZE_T count;
  AOC_T *entries;
} SET_NAME;

typedef struct SET_ITER_NAME {
  const SET_NAME *set;
  AOC_SIZE_T current;
} SET_ITER_NAME;

SET_LINKAGE void SET_CREATE(SET_NAME *const set, const AOC_SIZE_T capacity) {
  set->capacity = capacity;
  set->entries = (AOC_T *)aoc_alloc(sizeof(AOC_T) * set->capacity);
  SET_CLEAR(set);
}

SET_LINKAGE void SET_DESTROY(SET_NAME *set) {
  aoc_free(set->entries);
}

// TODO: similar to copy. maybe write a more general function
SET_LINKAGE void SET_ADJUST_CAP(SET_NAME *set, const AOC_SIZE_T capacity) {
  AOC_T *entries = (AOC_T *)aoc_alloc(sizeof(AOC_T) * capacity);
  for (AOC_SIZE_T i = 0; i < capacity; ++i)
    entries[i] = AOC_T_EMPTY;

  u32 hash = 0;
  AOC_SIZE_T index = 0;

  SET_ITER_NAME iter = {.set = set};
  AOC_T entry;
  while (SET_ITERATE(&iter, &entry)) {
    hash = AOC_T_HASH(&entry);
    index = SET_FIND_EMPTY(entries, capacity, hash);
    entries[index] = entry;
  }

  aoc_free(set->entries);
  set->entries = entries;
  set->capacity = capacity;
}

SET_LINKAGE void SET_ENSURE_CAP(SET_NAME *set) {
  if (set->count + 1 > set->capacity * SET_MAX_LOAD) {
    const AOC_SIZE_T capacity = set->capacity * 2;
    SET_ADJUST_CAP(set, capacity);
  }
}

SET_LINKAGE bool SET_FIND_ENTRY(AOC_T *keys, const AOC_SIZE_T capacity,
                                const AOC_T key, const u32 hash,
                                AOC_SIZE_T *const outIndex) {
  AOC_SIZE_T index = SET_MOD(hash, capacity);
  for (;;) {
    AOC_T *entry = &keys[index];
    if (AOC_T_EQUALS(entry, &AOC_T_EMPTY)) {
      return false;
    } else if (AOC_T_EQUALS(entry, &key)) {
      if (outIndex)
        *outIndex = index;
      return true;
    }
    index = SET_MOD(index + 1, capacity);
  }
  return false; // should never reach
}

SET_LINKAGE AOC_SIZE_T SET_FIND_EMPTY(AOC_T *keys, const AOC_SIZE_T capacity,
                                      const u32 hash) {
  AOC_SIZE_T index = SET_MOD(hash, capacity);
  for (;;) {
    if (AOC_T_EQUALS(&keys[index], &AOC_T_EMPTY))
      return index;
    index = SET_MOD(index + 1, capacity);
  }
  return 0; // should never reach
}

SET_LINKAGE void SET_INSERT(SET_NAME *const set, const AOC_T key) {
  SET_INSERT_PH(set, key, AOC_T_HASH(&key));
}

SET_LINKAGE void SET_INSERT_PH(SET_NAME *const set, const AOC_T key,
                               const u32 hash) {
  SET_ENSURE_CAP(set);
  const AOC_SIZE_T index = SET_FIND_EMPTY(set->entries, set->capacity, hash);
  set->entries[index] = key;
  set->count++;
}

SET_LINKAGE void SET_REMOVE(SET_NAME *const set, const AOC_T key) {
  SET_REMOVE_PH(set, key, AOC_T_HASH(&key));
}

SET_LINKAGE void SET_REMOVE_PH(SET_NAME *const set, const AOC_T key,
                               const u32 hash) {
  AOC_ASSERT(set->count > 0);
  AOC_SIZE_T index = 0;
  if (SET_FIND_ENTRY(set->entries, set->capacity, key, hash, &index)) {
    set->entries[index] = AOC_T_EMPTY;
    set->count--;
  }
}

SET_LINKAGE bool SET_CONTAINS(const SET_NAME *const set, const AOC_T key) {
  const u32 hash = AOC_T_HASH(&key);
  return SET_CONTAINS_PH(set, key, hash);
}

SET_LINKAGE bool SET_CONTAINS_PH(const SET_NAME *const set, const AOC_T key,
                                 const u32 hash) {
  return SET_FIND_ENTRY(set->entries, set->capacity, key, hash, NULL);
}

SET_LINKAGE void SET_CLEAR(SET_NAME *const set) {
  for (AOC_SIZE_T i = 0; i < set->capacity; ++i)
    set->entries[i] = AOC_T_EMPTY;
  set->count = 0;
}

SET_LINKAGE void SET_COPY(SET_NAME *const dest, const SET_NAME *const src) {
  AOC_ASSERT(dest->capacity >= src->capacity);
  if (dest->count > 0)
    SET_CLEAR(dest);

  u32 hash = 0;
  AOC_SIZE_T index = 0;

  SET_ITER_NAME iter = {.set = src};
  AOC_T entry;
  while (SET_ITERATE(&iter, &entry)) {
    hash = AOC_T_HASH(&entry);
    index = SET_FIND_EMPTY(dest->entries, dest->capacity, hash);
    dest->entries[index] = entry;
  }
  dest->count = src->count;
}

SET_LINKAGE void SET_DUPLICATE(SET_NAME *const dest,
                               const SET_NAME *const src) {
  SET_CREATE(dest, src->capacity);
  SET_COPY(dest, src);
}

SET_LINKAGE void SET_ITER_INIT(SET_ITER_NAME *const iter, SET_NAME *const set) {
  iter->set = set;
  iter->current = 0;
}

SET_LINKAGE bool SET_ITERATE(SET_ITER_NAME *const iter, AOC_T *const key) {
  if (iter->set->count == 0)
    return false;

  while (iter->current < iter->set->capacity &&
         AOC_T_EQUALS(&iter->set->entries[iter->current], &AOC_T_EMPTY)) {
    iter->current++;
  }

  if (iter->current == iter->set->capacity)
    return false;

  *key = iter->set->entries[iter->current];
  iter->current++;
  return true;
}

#endif

#ifndef AOC_INTERNAL_INCLUDE
#undef AOC_T
#undef AOC_T_NAME
#undef AOC_T_EMPTY
#undef AOC_T_HASH
#undef AOC_T_EQUALS

#ifdef AOC_BASE2_CAPACITY
#undef AOC_BASE2_CAPACITY
#endif

#ifdef AOC_SET_NO_IMPL
#undef AOC_SET_NO_IMPL
#endif

#undef COMBINE
#undef COMBINE2

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
#endif
