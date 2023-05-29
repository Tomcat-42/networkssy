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

// ack magic number
constexpr uint8_t ACK = 0x06;
constexpr uint8_t ACK_TRIES = 3;

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

auto socket::get_socketfd() const -> int {
  return socket_fd;
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

udp_socket::udp_socket() : socket(AF_INET, SOCK_DGRAM, 0) {}

auto udp_socket::set_destination(const std::string& host, uint16_t port)
  -> void {
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &address.sin_addr);

  if (::connect(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    throw std::runtime_error("Could not connect socket");
  }
}

tcp_connection::tcp_connection() = default;

auto tcp_connection::connect(const std::string& host, uint16_t port) -> void {
  socket.connect(host, port);
}

auto tcp_connection::disconnect() -> void {
  socket.close();
}

auto tcp_connection::send(const std::vector<uint8_t>& data) -> void {
  if (::send(socket.get_socketfd(), data.data(), data.size(), 0) < 0) {
    throw std::runtime_error("Could not send data");
  }
}

auto tcp_connection::receive(size_t size) -> std::vector<uint8_t> {
  std::vector<uint8_t> buffer(size);
  ssize_t received_bytes;

  if ((received_bytes = ::recv(socket.get_socketfd(), buffer.data(), size, 0)) <
      0) {
    throw std::runtime_error("Could not receive data");
  }
  buffer.resize(received_bytes);
  return buffer;
}

udp_connection::udp_connection() = default;

auto udp_connection::connect(const std::string& host, uint16_t port) -> void {
  socket.set_destination(host, port);
}

auto udp_connection::disconnect() -> void {
  socket.close();
}

auto udp_connection::send(const std::vector<uint8_t>& data) -> void {
  if (::send(socket.get_socketfd(), data.data(), data.size(), 0) < 0) {
    throw std::runtime_error("Could not send data");
  }
}

auto udp_connection::receive(size_t size) -> std::vector<uint8_t> {
  std::vector<uint8_t> buffer(size);
  ssize_t received_bytes;

  if ((received_bytes = ::recv(socket.get_socketfd(), buffer.data(), size, 0)) <
      0) {
    throw std::runtime_error("Could not receive data");
  }
  buffer.resize(received_bytes);
  return buffer;
}

auto udp_connection::send_with_ack(const std::vector<uint8_t>& data) -> void {
  // send data and wait for ack
  for (uint8_t i = 0; i < ACK_TRIES; i++) {
    send(data);
    std::vector<uint8_t> ack = receive(1);
    if (ack[0] == ACK) {
      return;
    }
  }
}

auto udp_connection::receive_with_ack(size_t size) -> std::vector<uint8_t> {
  // receive data and send ack
  std::vector<uint8_t> data = receive(size);
  std::vector<uint8_t> ack = {ACK};
  send(ack);
  return data;
}

auto fn() -> std::string {
  return "fn";
}
} // namespace networkssy
