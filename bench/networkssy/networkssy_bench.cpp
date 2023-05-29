#include <benchmark/benchmark.h>
#include <iostream>
#include <networkssy/networkssy.hpp>

static void BM_networkssy(benchmark::State &state) {
  for (auto _ : state) {
    networkssy::fn();
  }
}

BENCHMARK(BM_networkssy);
