// #pragma once
//
// #include <networkssy/socket.hpp>
//
// namespace networkssy::socket {
//
// class UdpSocket : public Socket {
// public:
//   UdpSocket();
//   ~UdpSocket();
//
//   auto set_destination(const std::string& host, uint16_t port) -> void;
//   auto receive_from(size_t size)
//     -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>>;
//   auto send_to(const std::vector<uint8_t>& data, const std::string& host,
//                uint16_t port) -> void;
// };
//
// } // namespace networkssy::socket
