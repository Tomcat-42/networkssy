// #include <iostream>
// #include <networkssy/connection/tcp.hpp>
// #include <sys/socket.h>
//
// namespace networkssy::connection {
//
// TcpConnection::TcpConnection() = default;
//
// // auto TcpConnection::connect(const std::string& host, uint16_t port) -> void
// // {
// //   socket.connect(host, port);
// // }
//
// auto TcpConnection::disconnect() -> void {
//   socket.close();
// }
//
// auto TcpConnection::send(const std::vector<uint8_t>& data) -> void {
//   if (::send(socket.get_socketfd(), data.data(), data.size(), 0) < 0) {
//     throw std::runtime_error("Could not send data");
//   }
// }
//
// auto TcpConnection::receive(size_t size) -> std::vector<uint8_t> {
//   std::vector<uint8_t> buffer(size);
//   ssize_t received_bytes;
//
//   if ((received_bytes = ::recv(socket.get_socketfd(), buffer.data(), size, 0)) <
//       0) {
//     throw std::runtime_error("Could not receive data");
//   }
//   buffer.resize(received_bytes);
//   return buffer;
// }
//
// auto TcpConnection::connect_to_server(const std::string& host, uint16_t port)
//   -> void {
//   socket.connect(host, port);
// }
//
// auto TcpConnection::start_server(uint16_t port) -> void {
//   socket.bind("0.0.0.0", port); // bind to all interfaces
//   socket.listen(10);            // listen with a backlog of 10
// }
//
// auto TcpConnection::accept_client() -> void {
//   socket.set_socketfd(socket.accept());
//   std::cout << "Client connected" << std::endl;
// }
//
// } // namespace networkssy::connection
