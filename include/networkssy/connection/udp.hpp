#pragma once

#include <networkssy/connection.hpp>
#include <networkssy/socket/udp.hpp>

namespace networkssy::connection {

class UdpConnection : public Connection {
public:
  UdpConnection();

  auto connect_to_server(const std::string& host, uint16_t port)
    -> void override;
  auto start_server(uint16_t port) -> void override;
  auto accept_client() -> void override;
  auto disconnect() -> void override;
  auto send(const std::vector<uint8_t>& data) -> void override;
  auto receive(size_t size) -> std::vector<uint8_t> override;
  auto send_to(const std::vector<uint8_t>& data, const std::string& host,
               uint16_t port) -> void;
  auto receive_from(size_t size)
    -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>>;
  auto send_with_ack(const std::vector<uint8_t>& data, const std::string& host,
                     uint16_t port) -> void;
  auto receive_with_ack(size_t size)
    -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>>;
  ~UdpConnection() override = default;

private:
  socket::UdpSocket socket;
  static constexpr uint8_t ACK = 0x01;
  static constexpr size_t ACK_SIZE = 1;
  static constexpr uint8_t ACK_TIMEOUT = 3;
  static constexpr uint8_t RETRY_DELAY_MS = 100;
};

} // namespace networkssy::connection
