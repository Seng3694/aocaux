#include "common.h"

// these macros have to be defined
// #define AOC_T
// #define AOC_T_NAME
// #define AOC_T_EMPTY
// #define AOC_T_HFUNC(x)
// #define AOC_T_EQUALS(a, b)

#ifndef AOC_T
#error "AOC_T must be defined"
#endif

#ifndef AOC_T_NAME
#error "AOC_T_NAME must be defined"
#endif

#ifndef AOC_T_EMPTY
#error "AOC_T_EMPTY must be defined"
#endif

#ifndef AOC_T_HFUNC
#error "AOC_T_HFUNC must be defined"
#endif

#ifndef AOC_T_EQUALS
#error "AOC_T_EQUALS must be defined"
#endif

#ifdef AOC_BASE2_CAPACITY
#define HS_MOD(a, b) ((a) & ((b)-1))
#else
#define HS_MOD(a, b) ((a) % (b))
#endif

#ifndef AOC_SIZE_T
#include <stddef.h>
#define AOC_SIZE_T size_t
#endif

#ifndef AOC_MALLOC
#include <stdlib.h>
#define AOC_MALLOC malloc
#endif

#ifndef AOC_FREE
#include <stdlib.h>
#define AOC_FREE free
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define HS_NAME COMBINE2(AocHashset, AOC_T_NAME)
#define HS_IMPL(word) COMBINE2(HS_NAME, word)
#define HS_IMPL_INTERNAL(word) COMBINE2(_, COMBINE2(HS_NAME, word))
#define HS_LINKAGE static inline
#define HS_MAX_LOAD 0.75

typedef struct {
  AOC_SIZE_T capacity;
  AOC_SIZE_T count;
  AOC_T *entries;
} HS_NAME;

#define HS_CREATE HS_IMPL(Create)
#define HS_DESTROY HS_IMPL(Destroy)
#define HS_ADJUST_CAP HS_IMPL_INTERNAL(AdjustCapacity)
#define HS_INSERT_INTERNAL HS_IMPL_INTERNAL(Insert)
#define HS_INSERT HS_IMPL(Insert)
#define HS_INSERT_PH HS_IMPL(InsertPreHashed)
#define HS_REMOVE HS_IMPL(Remove)
#define HS_REMOVE_PH HS_IMPL(RemovePreHashed)
#define HS_CLEAR HS_IMPL(Clear)
#define HS_CONTAINS HS_IMPL(Contains)

HS_LINKAGE void HS_CREATE(HS_NAME *const hs, const AOC_SIZE_T capacity);
HS_LINKAGE void HS_DESTROY(HS_NAME *hs);
HS_LINKAGE void HS_ADJUST_CAP(HS_NAME *hs, const AOC_SIZE_T capacity);

HS_LINKAGE void HS_INSERT_INTERNAL(AOC_T *entries, const AOC_SIZE_T capacity,
                                   const AOC_T key, const uint32_t *hash);
HS_LINKAGE void HS_INSERT(HS_NAME *const hs, const AOC_T key);
HS_LINKAGE void HS_INSERT_PH(HS_NAME *const hs, const AOC_T key,
                             const uint32_t hash);

HS_LINKAGE void HS_REMOVE(HS_NAME *const hs, const AOC_T key);
HS_LINKAGE void HS_REMOVE_PH(HS_NAME *const hs, const AOC_T key,
                             const uint32_t hash);

HS_LINKAGE void HS_CLEAR(HS_NAME *const hs);

HS_LINKAGE bool HS_CONTAINS(const HS_NAME *const hs, const AOC_T key,
                            uint32_t *hash);

HS_LINKAGE void HS_CREATE(HS_NAME *const hs, const AOC_SIZE_T capacity) {
  hs->capacity = capacity;
  hs->count = 0;
  hs->entries = (AOC_T *)AOC_MALLOC(sizeof(AOC_T) * hs->capacity);
  for (AOC_SIZE_T i = 0; i < capacity; ++i) {
    hs->entries[i] = AOC_T_EMPTY;
  }
}

HS_LINKAGE void HS_DESTROY(HS_NAME *hs) {
  AOC_FREE(hs->entries);
}

HS_LINKAGE void HS_ADJUST_CAP(HS_NAME *hs, const AOC_SIZE_T capacity) {
  AOC_T *entries = (AOC_T *)AOC_MALLOC(sizeof(AOC_T) * capacity);
  for (AOC_SIZE_T i = 0; i < capacity; ++i) {
    entries[i] = AOC_T_EMPTY;
  }

  hs->count = 0;
  for (AOC_SIZE_T i = 0; i < hs->capacity; ++i) {
    AOC_T *entry = &hs->entries[i];
    if (AOC_T_EQUALS(entry, &AOC_T_EMPTY))
      continue;
    HS_INSERT_INTERNAL(entries, capacity, *entry, NULL);
    hs->count++;
  }

  AOC_FREE(hs->entries);
  hs->entries = entries;
  hs->capacity = capacity;
}

HS_LINKAGE void HS_INSERT_INTERNAL(AOC_T *entries, const AOC_SIZE_T capacity,
                                   const AOC_T key, const uint32_t *hash) {
  const uint32_t keyHash = hash ? *hash : AOC_T_HFUNC(&key);
  uint32_t index = HS_MOD(keyHash, capacity);
  for (;;) {
    AOC_T *entry = &entries[index];
    if (AOC_T_EQUALS(entry, &AOC_T_EMPTY)) {
      *entry = key;
      return;
    }
    index = HS_MOD(index + 1, capacity);
  }
}

HS_LINKAGE void HS_INSERT(HS_NAME *const hs, const AOC_T key) {
  if (hs->count + 1 > hs->capacity * HS_MAX_LOAD) {
    const AOC_SIZE_T capacity = hs->capacity * 2;
    HS_ADJUST_CAP(hs, capacity);
  }
  HS_INSERT_INTERNAL(hs->entries, hs->capacity, key, NULL);
  hs->count++;
}

HS_LINKAGE void HS_INSERT_PH(HS_NAME *const hs, const AOC_T key,
                             const uint32_t hash) {
  if (hs->count + 1 > hs->capacity * HS_MAX_LOAD) {
    const AOC_SIZE_T capacity = hs->capacity * 2;
    HS_ADJUST_CAP(hs, capacity);
  }
  HS_INSERT_INTERNAL(hs->entries, hs->capacity, key, &hash);
  hs->count++;
}

HS_LINKAGE void HS_REMOVE(HS_NAME *const hs, const AOC_T key) {
  const uint32_t keyHash = AOC_T_HFUNC(&key);
  HS_REMOVE_PH(hs, key, keyHash);
}

HS_LINKAGE void HS_REMOVE_PH(HS_NAME *const hs, const AOC_T key,
                             const uint32_t hash) {
  uint32_t index = HS_MOD(hash, hs->capacity);
  for (;;) {
    AOC_T *entry = &hs->entries[index];
    if (AOC_T_EQUALS(entry, &AOC_T_EMPTY)) {
      return;
    } else if (AOC_T_EQUALS(entry, &key)) {
      *entry = AOC_T_EMPTY;
      hs->count--;
      return;
    }
    index = HS_MOD(index + 1, hs->capacity);
  }
}

HS_LINKAGE void HS_CLEAR(HS_NAME *const hs) {
  for (AOC_SIZE_T i = 0; i < hs->capacity; ++i) {
    hs->entries[i] = AOC_T_EMPTY;
  }
  hs->count = 0;
}

HS_LINKAGE bool HS_CONTAINS(const HS_NAME *const hs, const AOC_T key,
                            uint32_t *hash) {
  const uint32_t keyHash = AOC_T_HFUNC(&key);
  if (hash)
    *hash = keyHash;
  uint32_t index = HS_MOD(keyHash, hs->capacity);
  for (;;) {
    AOC_T *entry = &hs->entries[index];
    if (AOC_T_EQUALS(entry, &AOC_T_EMPTY)) {
      return false;
    } else if (AOC_T_EQUALS(entry, &key)) {
      return true;
    }
    index = HS_MOD(index + 1, hs->capacity);
  }
}

#undef COMBINE
#undef COMBINE2

#undef AOC_T
#undef AOC_T_NAME
#undef AOC_T_EMPTY
#undef AOC_T_HFUNC
#undef AOC_T_EQUALS
#undef AOC_MALLOC
#undef AOC_FREE
#undef AOC_SIZE_T
#undef AOC_BASE2_CAPACITY

#undef HS_MOD
#undef HS_NAME
#undef HS_IMPL
#undef HS_IMPL_INTERNAL
#undef HS_LINKAGE
#undef HS_MAX_LOAD
#undef HS_CREATE
#undef HS_DESTROY
#undef HS_ADJUST_CAP
#undef HS_INSERT_INTERNAL
#undef HS_INSERT
#undef HS_INSERT_PH
#undef HS_REMOVE
#undef HS_REMOVE_PH
#undef HS_CLEAR
#undef HS_CONTAINS
