#define PICOBENCH_IMPLEMENT_WITH_MAIN
#include "picobench.hpp"
#include <random>
#include <vector>

#define Uint uint64_t

using function_under_test = Uint *(Uint *, Uint *, Uint, Uint);

extern "C" {
Uint *original_flatmap_get(Uint *keys, Uint *vals, Uint key, Uint max_size);
Uint *latereturn_flatmap_get(Uint *keys, Uint *vals, Uint key, Uint max_size);
Uint *vectoradd_flatmap_get(Uint *keys, Uint *vals, Uint key, Uint max_size);
Uint *vectoror_flatmap_get(Uint *keys, Uint *vals, Uint key, Uint max_size);
}

static Uint keys[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                      11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                      22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

static Uint vals[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                      11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                      22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

template <function_under_test FUT> static void b(picobench::state &s) {
  std::random_device rd{};
  auto gen = std::mt19937{rd()};
  auto distr = std::uniform_int_distribution<>(
      0, s.iterations() * 2 - 1); // 50% chance of missing.
  s.start_timer();
  for (Uint tuple_size = 0; tuple_size < 32; ++tuple_size) {
    for (int i = 0; i < 100000; ++i) {
      Uint needle = distr(gen); // A random key
      FUT(keys, vals, needle, tuple_size);
    }
  }
  s.stop_timer();
}

const std::vector<int> iters = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                                12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                                23, 24, 25, 26, 27, 28, 29, 30, 31, 32};

PICOBENCH(b<original_flatmap_get>).iterations(iters);
PICOBENCH(b<latereturn_flatmap_get>).iterations(iters);
// PICOBENCH(b<vectoradd_flatmap_get>).iterations(iters);
// PICOBENCH(b<vectoror_flatmap_get>).iterations(iters);
