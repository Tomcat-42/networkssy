#include <arpa/inet.h>
#include <netinet/in.h>
#include <networkssy/socket/tcp.hpp>
#include <sys/socket.h>

namespace networkssy::socket {

TcpSocket::TcpSocket() : Socket(AF_INET, SOCK_STREAM, 0) {}

TcpSocket::~TcpSocket() {
  close();
}

auto TcpSocket::connect(const std::string& host, uint16_t port) -> void {
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &address.sin_addr);

  if (::connect(*socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    throw std::runtime_error("Could not connect socket");
  }
}

auto TcpSocket::listen(int backlog) -> void {
  if (::listen(*socket_fd, backlog) < 0) {
    throw std::runtime_error("Could not listen on socket");
  }
}

auto TcpSocket::accept() -> int {
  int new_socket_fd;
  if ((new_socket_fd = ::accept(*socket_fd, nullptr, nullptr)) < 0) {
    throw std::runtime_error("Could not accept connection");
  }
  return new_socket_fd;
}

}; // namespace networkssy::socket
