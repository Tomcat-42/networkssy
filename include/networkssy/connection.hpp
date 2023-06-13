#pragma once

#include <string>

namespace networkssy {

class Connection {
public:
  virtual auto connect_to_server(const std::string& host, uint16_t port)
    -> void = 0;
  virtual auto start_server(uint16_t port) -> void = 0;
  virtual auto accept_client() -> void = 0;
  virtual auto disconnect() -> void = 0;
  virtual auto send(const std::vector<uint8_t>& data) -> void = 0;
  virtual auto receive(size_t size) -> std::vector<uint8_t> = 0;
  virtual ~Connection() = default;
};

} // namespace networkssy
