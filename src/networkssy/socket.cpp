#include <arpa/inet.h>
#include <netinet/in.h>
#include <networkssy/socket.hpp>
#include <sys/socket.h>
#include <unistd.h>

namespace networkssy {

Socket::Socket(const int domain, const int type, const int protocol) {
  const int _socket_fd = ::socket(domain, type, protocol);
  if (_socket_fd < 0) {
    throw std::runtime_error("Could not create socket");
  }

  socket_fd = std::unique_ptr<const int, std::function<void(const int*)>>(
    new int(_socket_fd), [](const int* socket_fd) {
      ::close(*socket_fd);
      delete socket_fd;
    });
}

Socket::~Socket() {
  close();
}

auto Socket::bind(const std::string& host, const uint16_t port) const -> void {
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &address.sin_addr);

  if (::bind(*socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    throw std::runtime_error("Could not bind socket");
  }
}

auto Socket::close() const -> void {
  ::close(*socket_fd);
}

auto Socket::get_socketfd() const -> int {
  return *socket_fd;
}

auto Socket::set_socketfd(int new_socketfd) -> void {
  socket_fd.reset(new int(new_socketfd));
}

} // namespace networkssy
