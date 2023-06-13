#include <iostream>
#include <networkssy/connection/udp.hpp>
#include <ostream>
#include <sys/socket.h>
#include <thread>

namespace networkssy::connection {

UdpConnection::UdpConnection() = default;

// auto UdpConnection::connect(const std::string& host, uint16_t port) -> void
// {
//   socket.set_destination(host, port);
// }

auto UdpConnection::disconnect() -> void {
  socket.close();
}

auto UdpConnection::send(const std::vector<uint8_t>& data) -> void {
  if (::send(socket.get_socketfd(), data.data(), data.size(), 0) < 0) {
    throw std::runtime_error("Could not send data");
  }
}

auto UdpConnection::receive(size_t size) -> std::vector<uint8_t> {
  std::vector<uint8_t> buffer(size);
  ssize_t received_bytes;

  if ((received_bytes = ::recv(socket.get_socketfd(), buffer.data(), size, 0)) <
      0) {
    throw std::runtime_error("Could not receive data");
  }
  buffer.resize(received_bytes);
  return buffer;
}

auto UdpConnection::connect_to_server(const std::string& host, uint16_t port)
  -> void {
  socket.set_destination(host, port);
}

auto UdpConnection::start_server(uint16_t port) -> void {
  socket.bind("0.0.0.0", port); // bind to all interfaces
                                // no listen call for UDP
}

auto UdpConnection::accept_client() -> void {}

auto UdpConnection::send_to(const std::vector<uint8_t>& data,
                            const std::string& host, uint16_t port) -> void {
  socket.send_to(data, host, port);
}

auto UdpConnection::receive_from(size_t size)
  -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>> {
  return socket.receive_from(size);
}

auto UdpConnection::send_with_ack(const std::vector<uint8_t>& data,
                                  const std::string& host, uint16_t port)
  -> void {
  for (uint8_t i = 0; i < ACK_TIMEOUT; i++) {
    send_to(data, host, port);
    std::cout << "Sent data, waiting for ack" << std::endl;

    auto received = receive_from(1);
    std::cout << "Received " << received.first.size() << " bytes" << std::endl;

    if (!received.first.empty() && received.first[0] == ACK) {
      return;
    }

    // If no ACK received, wait before next attempt
    std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_DELAY_MS));
  }
  throw std::runtime_error("Failed to receive ACK after multiple attempts.");
}

auto UdpConnection::receive_with_ack(size_t size)
  -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>> {
  auto received = receive_from(size);
  if (!received.first.empty()) {
    std::cout << "Received " << received.first.size() << " bytes" << std::endl;
    std::vector<uint8_t> ack = {ACK};
    send_to(ack, received.second.first, received.second.second);
  }
  return received;
}

} // namespace networkssy::connection
