#pragma once

#include <functional>
#include <memory>
#include <string>

namespace networkssy {

class Socket {
public:
  Socket(int domain, int type, int protocol);
  ~Socket();

  auto bind(const std::string& host, uint16_t port) const -> void;
  auto close() const -> void;
  [[nodiscard]] auto get_socketfd() const -> int;
  auto set_socketfd(int new_socketfd) -> void;

protected:
  std::unique_ptr<const int, std::function<void(const int*)>>
    socket_fd; // NOLINT
};

} // namespace networkssy
