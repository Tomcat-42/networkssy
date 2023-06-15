// #pragma once
//
// #include <networkssy/connection.hpp>
// #include <networkssy/socket/tcp.hpp>
// #include <string>
//
// namespace networkssy::connection {
//
// class TcpConnection : public Connection {
// public:
//   TcpConnection();
//
//   auto connect_to_server(const std::string& host, uint16_t port)
//     -> void override;
//   auto start_server(uint16_t port) -> void override;
//   auto accept_client() -> void override;
//   auto disconnect() -> void override;
//   auto send(const std::vector<uint8_t>& data) -> void override;
//   auto receive(size_t size) -> std::vector<uint8_t> override;
//   ~TcpConnection() override = default;
//
// private:
//   socket::TcpSocket socket;
// };
//
// } // namespace networkssy::connection
