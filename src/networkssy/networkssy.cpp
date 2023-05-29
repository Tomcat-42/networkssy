#include <networkssy/networkssy.hpp>
#include <string>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdexcept>

namespace networkssy {

socket::socket(int domain, int type, int protocol) {
  if ((socket_fd = ::socket(domain, type, protocol)) < 0) {
    throw std::runtime_error("Could not create socket");
  }
}

socket::~socket() {
  close();
}

auto socket::bind(const std::string& host, uint16_t port) const -> void {
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &address.sin_addr);

  if (::bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    throw std::runtime_error("Could not bind socket");
  }
}

auto socket::close() const -> void {
  ::close(socket_fd);
}

auto fn() -> std::string {
  return "fn";
}
} // namespace networkssy
