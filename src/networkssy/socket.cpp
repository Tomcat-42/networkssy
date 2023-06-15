#include <arpa/inet.h>
#include <cerrno>
#include <exception>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <utility>

#include <networkssy/error.hpp>
#include <networkssy/socket.hpp>

namespace networkssy::socket {

Socket::Socket(
  const Domain domain, const Protocol protocol,
  std::function<int(const int, const int, const int)>&& socket_constructor,
  std::function<void(const int)>&& socket_destructor)
  : domain(domain), protocol(protocol),
    socket_constructor(std::move(socket_constructor)),
    socket_destructor(std::move(socket_destructor)) {
  const int socket_fd = socket_constructor(static_cast<const int>(domain),
                                           static_cast<const int>(protocol), 0);

  if (socket_fd == error::INVALID_SOCKETFD) {
    throw std::runtime_error(error::ERROR_MAP.at(errno));
  }

  socket = std::unique_ptr<const int, const std::function<void(const int*)>>(
    new int(socket_fd), [&socket_destructor](const int* socket_fd) {
      socket_destructor(*socket_fd);
      delete socket_fd;
    });
}

Socket::~Socket() {
  close();
}

auto Socket::close() -> void {
  if (socket) {
    socket.reset();
  }
}

auto Socket::get_socketfd() const -> int {
  if (!socket) {
    throw std::runtime_error(error::ERROR_MAP.at(error::INVALID_SOCKETFD));
  }
  return *socket;
}

auto Socket::set_socketfd(int new_socketfd) -> void {
  if (new_socketfd <= error::INVALID_SOCKETFD) {
    throw std::runtime_error(error::ERROR_MAP.at(error::INVALID_SOCKETFD));
  }
  socket.reset(new int(new_socketfd));
}

} // namespace networkssy::socket
