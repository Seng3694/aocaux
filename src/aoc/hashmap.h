#include "common.h"
#include "mem.h"

// these macros have to be defined
// #define AOC_KEY_T
// #define AOC_VALUE_T
// #define AOC_KEY_T_EMPTY
// #define AOC_KEY_T_HFUNC
// #define AOC_KEY_T_EQUALS

// this macros are optional
// #define AOC_KEY_T_NAME
// #define AOC_VALUE_T_NAME
// #define AOC_BASE2_CAPACITY

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

#ifndef AOC_KEY_T_HFUNC
#error "AOC_KEY_T_HFUNC must be defined"
#endif

#ifndef AOC_KEY_T_EQUALS
#error "AOC_KEY_T_EQUALS must be defined"
#endif

#ifdef AOC_BASE2_CAPACITY
#define HM_MOD(a, b) ((a) & ((b)-1))
#else
#define HM_MOD(a, b) ((a) % (b))
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define HM_NAME                                                                \
  COMBINE2(COMBINE2(COMBINE2(aoc_map_, AOC_KEY_T_NAME), _), AOC_VALUE_T_NAME)
#define HM_IMPL(word) COMBINE2(COMBINE2(HM_NAME, _), word)
#define HM_IMPL_INTERNAL(word) COMBINE2(_, COMBINE2(COMBINE2(HM_NAME, _), word))
#define HM_LINKAGE static inline
#define HM_MAX_LOAD 0.75

typedef struct {
  AOC_SIZE_T capacity;
  AOC_SIZE_T count;
  AOC_KEY_T *keys;
  AOC_VALUE_T *values;
} HM_NAME;

#define HM_CREATE HM_IMPL(create)
#define HM_DESTROY HM_IMPL(destroy)
#define HM_ADJUST_CAP HM_IMPL_INTERNAL(adjust_capacity)
#define HM_INSERT_INTERNAL HM_IMPL_INTERNAL(insert)
#define HM_INSERT HM_IMPL(insert)
#define HM_GET HM_IMPL(get)
#define HM_GET_PH HM_IMPL(get_pre_hashed)
#define HM_INSERT_PH HM_IMPL(insert_pre_hashed)
#define HM_REMOVE HM_IMPL(remove)
#define HM_REMOVE_PH HM_IMPL(remove_pre_hashed)
#define HM_CLEAR HM_IMPL(clear)
#define HM_CONTAINS HM_IMPL(contains)

HM_LINKAGE void HM_CREATE(HM_NAME *const hm, const AOC_SIZE_T capacity);
HM_LINKAGE void HM_DESTROY(HM_NAME *hm);
HM_LINKAGE void HM_ADJUST_CAP(HM_NAME *hm, const AOC_SIZE_T capacity);

HM_LINKAGE void HM_INSERT_INTERNAL(AOC_KEY_T *keys, AOC_VALUE_T *values,
                                   const AOC_SIZE_T capacity,
                                   const AOC_KEY_T key, AOC_VALUE_T value,
                                   const u32 *hash);

HM_LINKAGE void HM_INSERT(HM_NAME *const hm, const AOC_KEY_T key,
                          AOC_VALUE_T value);

HM_LINKAGE void HM_INSERT_PH(HM_NAME *const hm, const AOC_KEY_T key,
                             AOC_VALUE_T value, const u32 hash);

HM_LINKAGE bool HM_GET(const HM_NAME *const hm, const AOC_KEY_T key,
                       AOC_VALUE_T *out);

HM_LINKAGE bool HM_GET_PH(const HM_NAME *const hm, const AOC_KEY_T key,
                          const u32 hash, AOC_VALUE_T *out);

HM_LINKAGE void HM_REMOVE(HM_NAME *const hm, const AOC_KEY_T key);
HM_LINKAGE void HM_REMOVE_PH(HM_NAME *const hm, const AOC_KEY_T key,
                             const u32 hash);

HM_LINKAGE void HM_CLEAR(HM_NAME *const hm);

HM_LINKAGE bool HM_CONTAINS(const HM_NAME *const hm, const AOC_KEY_T key,
                            u32 *hash);

HM_LINKAGE void HM_CREATE(HM_NAME *const hm, const AOC_SIZE_T capacity) {
  hm->capacity = capacity;
  hm->count = 0;
  hm->keys = (AOC_KEY_T *)aoc_alloc(sizeof(AOC_KEY_T) * hm->capacity);
  hm->values = (AOC_VALUE_T *)aoc_alloc(sizeof(AOC_VALUE_T) * hm->capacity);
  for (AOC_SIZE_T i = 0; i < capacity; ++i) {
    hm->keys[i] = AOC_KEY_T_EMPTY;
  }
}

HM_LINKAGE void HM_DESTROY(HM_NAME *hm) {
  aoc_free(hm->keys);
  aoc_free(hm->values);
}

HM_LINKAGE void HM_ADJUST_CAP(HM_NAME *hm, const AOC_SIZE_T capacity) {
  AOC_KEY_T *keys = (AOC_KEY_T *)aoc_alloc(sizeof(AOC_KEY_T) * capacity);
  AOC_VALUE_T *values =
      (AOC_VALUE_T *)aoc_alloc(sizeof(AOC_VALUE_T) * capacity);

  for (AOC_SIZE_T i = 0; i < capacity; ++i) {
    keys[i] = AOC_KEY_T_EMPTY;
  }

  hm->count = 0;
  for (AOC_SIZE_T i = 0; i < hm->capacity; ++i) {
    AOC_KEY_T *entry = &hm->keys[i];
    if (AOC_KEY_T_EQUALS(entry, &AOC_KEY_T_EMPTY))
      continue;
    AOC_VALUE_T *value = &hm->values[i];
    HM_INSERT_INTERNAL(keys, values, capacity, *entry, *value, NULL);
    hm->count++;
  }

  aoc_free(hm->keys);
  aoc_free(hm->values);
  hm->keys = keys;
  hm->values = values;
  hm->capacity = capacity;
}

HM_LINKAGE void HM_INSERT_INTERNAL(AOC_KEY_T *keys, AOC_VALUE_T *values,
                                   const AOC_SIZE_T capacity,
                                   const AOC_KEY_T key, AOC_VALUE_T value,
                                   const u32 *hash) {
  const u32 keyHash = hash ? *hash : AOC_KEY_T_HFUNC(&key);
  u32 index = HM_MOD(keyHash, capacity);
  for (;;) {
    AOC_KEY_T *entry = &keys[index];
    if (AOC_KEY_T_EQUALS(entry, &AOC_KEY_T_EMPTY)) {
      *entry = key;
      values[index] = value;
      return;
    }
    index = HM_MOD(index + 1, capacity);
  }
}

HM_LINKAGE void HM_INSERT(HM_NAME *const hm, const AOC_KEY_T key,
                          AOC_VALUE_T value) {
  if (hm->count + 1 > hm->capacity * HM_MAX_LOAD) {
    const AOC_SIZE_T capacity = hm->capacity * 2;
    HM_ADJUST_CAP(hm, capacity);
  }
  HM_INSERT_INTERNAL(hm->keys, hm->values, hm->capacity, key, value, NULL);
  hm->count++;
}

HM_LINKAGE void HM_INSERT_PH(HM_NAME *const hm, const AOC_KEY_T key,
                             AOC_VALUE_T value, const u32 hash) {
  if (hm->count + 1 > hm->capacity * HM_MAX_LOAD) {
    const AOC_SIZE_T capacity = hm->capacity * 2;
    HM_ADJUST_CAP(hm, capacity);
  }
  HM_INSERT_INTERNAL(hm->keys, hm->values, hm->capacity, key, value, &hash);
  hm->count++;
}

HM_LINKAGE bool HM_GET(const HM_NAME *const hm, const AOC_KEY_T key,
                       AOC_VALUE_T *out) {
  const u32 keyHash = AOC_KEY_T_HFUNC(&key);
  return HM_GET_PH(hm, key, keyHash, out);
}

HM_LINKAGE bool HM_GET_PH(const HM_NAME *const hm, const AOC_KEY_T key,
                          const u32 hash, AOC_VALUE_T *out) {
  u32 index = HM_MOD(hash, hm->capacity);
  for (;;) {
    AOC_KEY_T *entry = &hm->keys[index];
    if (AOC_KEY_T_EQUALS(entry, &AOC_KEY_T_EMPTY)) {
      return false;
    } else if (AOC_KEY_T_EQUALS(entry, &key)) {
      *out = hm->values[index];
      return true;
    }
    index = HM_MOD(index + 1, hm->capacity);
  }
}

HM_LINKAGE void HM_REMOVE(HM_NAME *const hm, const AOC_KEY_T key) {
  const u32 keyHash = AOC_KEY_T_HFUNC(&key);
  HM_REMOVE_PH(hm, key, keyHash);
}

HM_LINKAGE void HM_REMOVE_PH(HM_NAME *const hm, const AOC_KEY_T key,
                             const u32 hash) {
  u32 index = HM_MOD(hash, hm->capacity);
  for (;;) {
    AOC_KEY_T *entry = &hm->keys[index];
    if (AOC_KEY_T_EQUALS(entry, &AOC_KEY_T_EMPTY)) {
      return;
    } else if (AOC_KEY_T_EQUALS(entry, &key)) {
      *entry = AOC_KEY_T_EMPTY;
      hm->count--;
      return;
    }
    index = HM_MOD(index + 1, hm->capacity);
  }
}

HM_LINKAGE void HM_CLEAR(HM_NAME *const hm) {
  for (AOC_SIZE_T i = 0; i < hm->capacity; ++i) {
    hm->keys[i] = AOC_KEY_T_EMPTY;
  }
  hm->count = 0;
}

HM_LINKAGE bool HM_CONTAINS(const HM_NAME *const hm, const AOC_KEY_T key,
                            u32 *hash) {
  const u32 keyHash = AOC_KEY_T_HFUNC(&key);
  if (hash)
    *hash = keyHash;
  if (hm->count == 0)
    return false;
  u32 index = HM_MOD(keyHash, hm->capacity);
  for (;;) {
    AOC_KEY_T *entry = &hm->keys[index];
    if (AOC_KEY_T_EQUALS(entry, &AOC_KEY_T_EMPTY)) {
      return false;
    } else if (AOC_KEY_T_EQUALS(entry, &key)) {
      return true;
    }
    index = HM_MOD(index + 1, hm->capacity);
  }
}

#define HM_ITER_NAME                                                           \
  COMBINE2(COMBINE2(COMBINE2(aoc_map_iter_, AOC_KEY_T_NAME), _),               \
           AOC_VALUE_T_NAME)
#define HM_ITER_IMPL(word) COMBINE2(HM_ITER_NAME, COMBINE2(_, word))

typedef struct {
  HM_NAME *map;
  AOC_SIZE_T current;
} HM_ITER_NAME;

#define HM_ITER_INIT HM_ITER_IMPL(init)
#define HM_ITERATE HM_IMPL(iterate)

HM_LINKAGE void HM_ITER_INIT(HM_ITER_NAME *const iter, HM_NAME *const map);
HM_LINKAGE bool HM_ITERATE(HM_ITER_NAME *const iter, AOC_KEY_T *const key,
                           AOC_VALUE_T *const value);

HM_LINKAGE void HM_ITER_INIT(HM_ITER_NAME *const iter, HM_NAME *const map) {
  iter->map = map;
  iter->current = 0;
}

HM_LINKAGE bool HM_ITERATE(HM_ITER_NAME *const iter, AOC_KEY_T *const key,
                           AOC_VALUE_T *const value) {
  if (iter->map->count == 0)
    return false;

  while (iter->current < iter->map->capacity &&
         AOC_KEY_T_EQUALS(&iter->map->keys[iter->current], &AOC_KEY_T_EMPTY)) {
    iter->current++;
  }

  if (iter->current == iter->map->capacity)
    return false;

  *key = iter->map->keys[iter->current];
  *value = iter->map->values[iter->current];
  iter->current++;
  return true;
}

#undef AOC_KEY_T
#undef AOC_KEY_T_NAME
#undef AOC_VALUE_T
#undef AOC_VALUE_T_NAME
#undef AOC_KEY_T_EMPTY
#undef AOC_KEY_T_HFUNC
#undef AOC_KEY_T_EQUALS

#undef AOC_BASE2_CAPACITY

#undef COMBINE
#undef COMBINE2

#undef HM_MOD
#undef HM_NAME
#undef HM_IMPL
#undef HM_IMPL_INTERNAL
#undef HM_LINKAGE
#undef HM_MAX_LOAD

#undef HM_CREATE
#undef HM_DESTROY
#undef HM_ADJUST_CAP
#undef HM_INSERT_INTERNAL
#undef HM_INSERT
#undef HM_GET
#undef HM_GET_PH
#undef HM_INSERT_PH
#undef HM_REMOVE
#undef HM_REMOVE_PH
#undef HM_CLEAR
#undef HM_CONTAINS

#undef HM_ITER_NAME
#undef HM_ITER_IMPL
#undef HM_ITER_INIT
#undef HM_ITERATE
