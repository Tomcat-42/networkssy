#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace networkssy {

class connection {
public:
  virtual auto connect(const std::string& host, uint16_t port) -> void = 0;
  virtual auto disconnect() -> void = 0;
  virtual auto send(const std::vector<uint8_t>& data) -> void = 0;
  virtual auto receive(size_t size) -> std::vector<uint8_t> = 0;
};

class socket {
public:
  socket(int domain, int type, int protocol);
  ~socket();

  auto bind(const std::string& host, uint16_t port) const -> void;
  auto close() const -> void;
  [[nodiscard]] auto get_socketfd() const -> int;

protected:
  int socket_fd; // NOLINT
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
};

class tcp_connection : public connection {
public:
  tcp_connection();

  auto connect(const std::string& host, uint16_t port) -> void override;
  auto disconnect() -> void override;
  auto send(const std::vector<uint8_t>& data) -> void override;
  auto receive(size_t size) -> std::vector<uint8_t> override;

private:
  tcp_socket socket;
};

class udp_connection : public connection {
public:
  udp_connection();

  auto connect(const std::string& host, uint16_t port) -> void override;
  auto disconnect() -> void override;
  auto send(const std::vector<uint8_t>& data) -> void override;
  auto receive(size_t size) -> std::vector<uint8_t> override;

private:
  udp_socket socket;
};

} // namespace networkssy
