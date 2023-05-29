#include <networkssy/networkssy.hpp>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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

tcp_socket::tcp_socket() : socket(AF_INET, SOCK_STREAM, 0) {}

tcp_socket::~tcp_socket() {
  close();
}

auto tcp_socket::connect(const std::string& host, uint16_t port) -> void {
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &address.sin_addr);

  if (::connect(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    throw std::runtime_error("Could not connect socket");
  }
}

auto tcp_socket::listen(int backlog) -> void {
  if (::listen(socket_fd, backlog) < 0) {
    throw std::runtime_error("Could not listen on socket");
  }
}

auto tcp_socket::accept() -> int {
  int new_socket_fd;
  if ((new_socket_fd = ::accept(socket_fd, nullptr, nullptr)) < 0) {
    throw std::runtime_error("Could not accept connection");
  }
  return new_socket_fd;
}

auto fn() -> std::string {
  return "fn";
}
} // namespace networkssy
