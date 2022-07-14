#define PICOBENCH_IMPLEMENT_WITH_MAIN
#include "picobench.hpp"
#include <random>

#define Uint uint64_t

typedef Uint *(*function_under_test_t)(Uint *, Uint *, Uint, Uint);

#define foo_ptr ()

extern Uint *original_map_get(Uint *keys, Uint *vals, Uint key, Uint max_size);
extern Uint *latereturn_map_get(Uint *keys, Uint *vals, Uint key,
                                Uint max_size);
extern Uint *vectoradd_map_get(Uint *keys, Uint *vals, Uint key, Uint max_size);
extern Uint *vectoror_map_get(Uint *keys, Uint *vals, Uint key, Uint max_size);

static Uint keys[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                      11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                      22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

static Uint vals[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                      11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                      22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

template <function_under_test_t FUT>
static void benchmark(picobench::state &s) {
  std::random_device rd{};
  auto gen = std::mt19937{rd()};
  auto distr = std::uniform_int_distribution<>(0, 31);
  s.start_timer();
  for (Uint tuple_size = 0; tuple_size < 32; ++tuple_size) {
    for (auto _ : s) {
      Uint needle = distr(gen); // A random key
      FUT(keys, vals, needle, tuple_size);
    }
  }
  s.stop_timer();
}

PICOBENCH(benchmark<original_map_get>);
PICOBENCH(benchmark<latereturn_map_get>);
PICOBENCH(benchmark<vectoradd_map_get>);
PICOBENCH(benchmark<vectoror_map_get>);
