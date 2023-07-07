#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace networkssy {

class connection {
public:
  virtual auto connect_to_server(const std::string& host, uint16_t port)
    -> void = 0;
  virtual auto start_server(uint16_t port) -> void = 0;
  virtual auto accept_client() -> void = 0;
  virtual auto disconnect() -> void = 0;
  virtual auto send(const std::vector<uint8_t>& data) -> void = 0;
  virtual auto receive(size_t size) -> std::vector<uint8_t> = 0;
  virtual ~connection() = default;
};

class socket {
public:
  socket(int domain, int type, int protocol);
  ~socket();

  auto bind(const std::string& host, uint16_t port) const -> void;
  auto close() const -> void;
  [[nodiscard]] auto get_socketfd() const -> int;
  auto set_socketfd(int new_socketfd) -> void;

protected:
  std::unique_ptr<const int, std::function<void(const int*)>>
    socket_fd; // NOLINT
};

class tcp_socket : public socket {
public:
  tcp_socket();
  ~tcp_socket();

  auto connect(const std::string& host, uint16_t port) -> void;
  auto listen(int backlog) -> void;
  auto accept() -> int;
};

class udp_socket : public socket {
public:
  udp_socket();
  ~udp_socket();

  auto set_destination(const std::string& host, uint16_t port) -> void;
  auto receive_from(size_t size)
    -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>>;
  auto send_to(const std::vector<uint8_t>& data, const std::string& host,
               uint16_t port) -> void;
};

class tcp_connection : public connection {
public:
  tcp_connection();

  auto connect_to_server(const std::string& host, uint16_t port)
    -> void override;
  auto start_server(uint16_t port) -> void override;
  auto accept_client() -> void override;
  auto disconnect() -> void override;
  auto send(const std::vector<uint8_t>& data) -> void override;
  auto receive(size_t size) -> std::vector<uint8_t> override;
  ~tcp_connection() override = default;

private:
  tcp_socket socket;
};

class udp_connection : public connection {
public:
  udp_connection();

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
                     uint16_t port) -> size_t;
  auto receive_with_ack(size_t size)
    -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>>;
  ~udp_connection() override = default;

private:
  udp_socket socket;
  static constexpr uint8_t ACK = 0x01;
  static constexpr size_t ACK_SIZE = 1;
  static constexpr uint8_t ACK_TIMEOUT = 3;
  static constexpr uint8_t RETRY_DELAY_MS = 100;
};
} // namespace networkssy
