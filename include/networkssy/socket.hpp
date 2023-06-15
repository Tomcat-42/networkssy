#pragma once

#include <functional>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace networkssy::socket {

enum class Protocol : const int {
  TCP = SOCK_STREAM,
  UDP = SOCK_DGRAM,
};

enum class Domain : const int {
  IPV4 = AF_INET,
  IPV6 = AF_INET6,
  UNIX = AF_UNIX
};

class Socket {
protected:
  Domain domain; //NOLINT
  Protocol protocol; //NOLINT
  std::unique_ptr<const int, std::function<void(const int*)>> socket = nullptr; //NOLINT
  std::function<int(const int, const int, const int)>&& socket_constructor; //NOLINT
  std::function<void(const int)>&& socket_destructor; //NOLINT

public:
  Socket(Domain domain, Protocol protocol,
         std::function<int(const int, const int, const int)>&&
           socket_constructor = ::socket,
         std::function<void(const int)>&& socket_destructor = ::close);
  ~Socket();

  auto close() -> void;
  [[nodiscard]] auto get_socketfd() const -> int;
  auto set_socketfd(int new_socketfd) -> void;
};

} // namespace networkssy::socket
