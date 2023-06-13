#pragma once
#include <networkssy/socket.hpp>

namespace networkssy::socket {

class TcpSocket : public Socket {
public:
  TcpSocket();
  ~TcpSocket();

  auto connect(const std::string& host, uint16_t port) -> void;
  auto listen(int backlog) -> void;
  auto accept() -> int;
};

} // namespace networkssy::socket
