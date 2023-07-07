#include <cstddef>
#include <iostream>
#include <networkssy/networkssy.hpp>

#include <arpa/inet.h>
#include <cmath>
#include <netdb.h>
#include <netinet/in.h>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

namespace networkssy {

socket::socket(const int domain, const int type, const int protocol) {
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

socket::~socket() {
  close();
}

auto socket::bind(const std::string& host, const uint16_t port) const -> void {
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &address.sin_addr);

  if (::bind(*socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    throw std::runtime_error("Could not bind socket");
  }
}

auto socket::close() const -> void {
  ::close(*socket_fd);
}

auto socket::get_socketfd() const -> int {
  return *socket_fd;
}

auto socket::set_socketfd(int new_socketfd) -> void {
  socket_fd.reset(new int(new_socketfd));
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

  if (::connect(*socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    throw std::runtime_error("Could not connect socket");
  }
}

auto tcp_socket::listen(int backlog) -> void {
  if (::listen(*socket_fd, backlog) < 0) {
    throw std::runtime_error("Could not listen on socket");
  }
}

auto tcp_socket::accept() -> int {
  int new_socket_fd;
  if ((new_socket_fd = ::accept(*socket_fd, nullptr, nullptr)) < 0) {
    throw std::runtime_error("Could not accept connection");
  }
  return new_socket_fd;
}

udp_socket::udp_socket() : socket(AF_INET, SOCK_DGRAM, 0) {}

udp_socket::~udp_socket() = default;

auto udp_socket::set_destination(const std::string& host, uint16_t port)
  -> void {
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &address.sin_addr);

  if (::connect(*socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    throw std::runtime_error("Could not connect socket");
  }
}

auto udp_socket::receive_from(size_t size)
  -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>> {
  std::vector<uint8_t> buffer(size);
  struct sockaddr_in sender_addr;
  socklen_t addr_size = sizeof(sender_addr);

  ssize_t received_bytes = recvfrom(get_socketfd(), buffer.data(), size, 0,
                                    (struct sockaddr*)&sender_addr, &addr_size);
  if (received_bytes == -1) {
    // Handle error
  }

  std::string sender_ip(inet_ntoa(sender_addr.sin_addr));
  uint16_t sender_port = ntohs(sender_addr.sin_port);

  return {buffer, {sender_ip, sender_port}};
}

auto udp_socket::send_to(const std::vector<uint8_t>& data,
                         const std::string& host, uint16_t port) -> void {
  struct sockaddr_in recipient_addr;
  recipient_addr.sin_family = AF_INET;
  recipient_addr.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &recipient_addr.sin_addr);

  if (sendto(get_socketfd(), data.data(), data.size(), 0,
             (struct sockaddr*)&recipient_addr, sizeof(recipient_addr)) < 0) {
    throw std::runtime_error("Could not send data");
  }
}

tcp_connection::tcp_connection() = default;

// auto tcp_connection::connect(const std::string& host, uint16_t port) -> void
// {
//   socket.connect(host, port);
// }

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

auto tcp_connection::connect_to_server(const std::string& host, uint16_t port)
  -> void {
  socket.connect(host, port);
}

auto tcp_connection::start_server(uint16_t port) -> void {
  socket.bind("0.0.0.0", port); // bind to all interfaces
  socket.listen(10);            // listen with a backlog of 10
}

auto tcp_connection::accept_client() -> void {
  socket.set_socketfd(socket.accept());
  std::cout << "Client connected" << std::endl;
}

udp_connection::udp_connection() = default;

// auto udp_connection::connect(const std::string& host, uint16_t port) -> void
// {
//   socket.set_destination(host, port);
// }

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

auto udp_connection::connect_to_server(const std::string& host, uint16_t port)
  -> void {
  socket.set_destination(host, port);
}

auto udp_connection::start_server(uint16_t port) -> void {
  socket.bind("0.0.0.0", port); // bind to all interfaces
                                // no listen call for UDP
}

auto udp_connection::accept_client() -> void {}

auto udp_connection::send_to(const std::vector<uint8_t>& data,
                             const std::string& host, uint16_t port) -> void {
  socket.send_to(data, host, port);
}

auto udp_connection::receive_from(size_t size)
  -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>> {
  return socket.receive_from(size);
}

auto udp_connection::send_with_ack(const std::vector<uint8_t>& data,
                                   const std::string& host, uint16_t port)
  -> size_t {

  size_t number_of_retries = 0;

  for (uint8_t i = 0; i < ACK_TIMEOUT; i++) {
    send_to(data, host, port);
    std::cout << "Sent data, waiting for ack" << std::endl;

    auto received = receive_from(1);
    std::cout << "Received " << received.first.size() << " bytes" << std::endl;

    // data was sent and ACK received
    if (!received.first.empty() && received.first[0] == ACK) {
      return number_of_retries;
    }

    // If no ACK received, wait before next attempt
    number_of_retries++;
    std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_DELAY_MS));
  }
  throw std::runtime_error("Failed to receive ACK after multiple attempts.");
}

auto udp_connection::receive_with_ack(size_t size)
  -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>> {
  auto received = receive_from(size);
  if (!received.first.empty()) {
    std::cout << "Received " << received.first.size() << " bytes" << std::endl;
    std::vector<uint8_t> ack = {ACK};
    send_to(ack, received.second.first, received.second.second);
  }
  return received;
}

auto fn() -> std::string {
  return "fn";
}
} // namespace networkssy
