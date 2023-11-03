#include "common.h"
#include <stdlib.h>

#ifndef AOC_T
#error "AOC_T must be defined"
#endif

#ifndef AOC_POINT_COMPONENTS
#define AOC_POINT_COMPONENTS 2
#else
#if AOC_POINT_COMPONENTS < 2 || AOC_POINT_COMPONENTS > 4
#error "AOC_POINT_COMPONENTS has to be between 2 and 4"
#endif
#endif

#ifndef AOC_T_NAME
#define AOC_T_NAME AOC_T
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define POINT_NAME                                                             \
  COMBINE2(COMBINE2(COMBINE2(aoc_point, AOC_POINT_COMPONENTS), _), AOC_T_NAME)
#define POINT_IMPL(word) COMBINE2(POINT_NAME, COMBINE2(_, word))
#define POINT_LINKAGE static inline

typedef struct {
  AOC_T x;
  AOC_T y;
#if AOC_POINT_COMPONENTS > 2
  AOC_T z;
#endif
#if AOC_POINT_COMPONENTS == 4
  AOC_T w;
#endif
} POINT_NAME;

// use void pointer for compatability with qsort
POINT_LINKAGE int POINT_IMPL(compare)(const void *const a,
                                      const void *const b) {
  const POINT_NAME *const ap = a;
  const POINT_NAME *const bp = b;
  const i64 xdiff = (i64)ap->x - (i64)bp->x;
  const i64 ydiff = (i64)ap->y - (i64)bp->y;
#if AOC_POINT_COMPONENTS > 2
  const i64 zdiff = (i64)ap->z - (i64)bp->z;
#endif
#if AOC_POINT_COMPONENTS == 4
  const i64 wdiff = (i64)ap->w - (i64)bp->w;
#endif

#if AOC_POINT_COMPONENTS == 4
  if (wdiff == 0) {
#endif

#if AOC_POINT_COMPONENTS > 2
    if (zdiff == 0) {
#endif
      return ydiff == 0 ? xdiff : ydiff;
#if AOC_POINT_COMPONENTS > 2
    } else {
      return 0;
    }
#endif

#if AOC_POINT_COMPONENTS == 4
  } else {
    return 0;
  }
#endif

  //   if (xdiff == 0) {
  // #if AOC_POINT_COMPONENTS > 2
  //     if (ydiff == 0) {
  // #if AOC_POINT_COMPONENTS == 4
  //       return zdiff == 0 ? wdiff : zdiff;
  // #else
  //       return zdiff;
  // #endif
  //     } else {
  //       return ydiff;
  //     }
  // #else
  //     return ydiff;
  // #endif
  //   } else {
  //     return xdiff;
  //   }
}

POINT_LINKAGE u32 POINT_IMPL(hash)(const POINT_NAME *const p) {
  u32 hash = 36591911;
  hash = ((hash << 5) + hash) + p->x;
  hash = ((hash << 5) + hash) + p->y;
#if AOC_POINT_COMPONENTS > 2
  hash = ((hash << 5) + hash) + p->z;
#endif
#if AOC_POINT_COMPONENTS == 4
  hash = ((hash << 5) + hash) + p->w;
#endif
  return hash;
}

POINT_LINKAGE bool POINT_IMPL(equals)(const POINT_NAME *const a,
                                      const POINT_NAME *const b) {
  return a->x == b->x && a->y == b->y
#if AOC_POINT_COMPONENTS > 2
         && a->z == b->z
#endif
#if AOC_POINT_COMPONENTS == 4
         && a->w == b->w
#endif
      ;
}

POINT_LINKAGE AOC_T POINT_IMPL(taxicab)(const POINT_NAME *const a,
                                        const POINT_NAME *const b) {
  return (AOC_T)(abs((int)a->x - (int)b->x) + abs((int)a->y - (int)b->y)
#if AOC_POINT_COMPONENTS > 2
                 + abs((int)a->z - (int)b->z)
#endif
#if AOC_POINT_COMPONENTS == 4
                 + abs((int)a->w - (int)b->w)
#endif
  );
}

#undef AOC_T
#undef AOC_POINT_COMPONENTS
#undef AOC_T_NAME
#undef COMBINE
#undef COMBINE2
#undef POINT_NAME
#undef POINT_IMPL
#undef POINT_LINKAGE
