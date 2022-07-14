#include <stddef.h>
#include <stdint.h>

#define min(a, b) ((a) < (b) ? a : b)
#define Uint uint64_t

Uint *original_map_get(Uint *keys, Uint *vals, Uint key, Uint max_size) {
  Uint n = max_size;
  Uint i;

  for (i = 0; i < n; ++i) {
    if (keys[i] == key) {
      return &vals[i];
    }
  }
  return NULL;
}

Uint *latereturn_flatmap_get(Uint *keys, Uint *vals, Uint key, Uint max_size) {
  Uint n = min(max_size, 32);
  Uint i;

  Uint *res = NULL;
  for (i = 0; i < n; ++i) {
    if (keys[i] == key) {
      res = &vals[i];
    }
  }
  return res;
}

Uint *vectoradd_flatmap_get(Uint *keys, Uint *vals, Uint key, Uint max_size) {
  Uint n = min(max_size, 32);
  Uint i;

  Uint res = 0;
  for (i = 0; i < n; ++i) {
    if (keys[i] == key) {
      res += (i + 1);
    }
  }
  return res ? &vals[res - 1] : NULL;
}

Uint *vectoror_flatmap_get(Uint *keys, Uint *vals, Uint key, Uint max_size) {
  Uint n = min(max_size, 32);
  Uint i;

  Uint res = 0;
  for (i = 0; i < n; ++i) {
    if (keys[i] == key) {
      res |= (i + 1);
    }
  }
  return res ? &vals[res - 1] : NULL;
}
