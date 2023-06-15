#include <benchmark/benchmark.h>
#include <networkssy/socket.hpp>

using namespace networkssy::socket;

static constexpr const int VALID_SOCKET_FD = 10;

static void BM_Socket_Construct(benchmark::State& state) {
  for (auto state : state) {
    Socket socket(Domain::IPV4, Protocol::TCP);
    benchmark::DoNotOptimize(socket);
  }
}

static void BM_Socket_SetFd(benchmark::State& state) {
  for (auto state : state) {
    Socket socket(Domain::IPV4, Protocol::TCP);
    socket.set_socketfd(VALID_SOCKET_FD);
    benchmark::DoNotOptimize(socket.get_socketfd());
  }
}

static void BM_Socket_Close(benchmark::State& state) {
  for (auto state : state) {
    Socket socket(Domain::IPV4, Protocol::TCP);
    socket.close();
  }
}

BENCHMARK(BM_Socket_SetFd);
BENCHMARK(BM_Socket_Construct);
BENCHMARK(BM_Socket_Close);
