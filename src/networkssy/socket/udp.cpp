// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <networkssy/socket/udp.hpp>
// #include <sys/socket.h>
//
// namespace networkssy::socket {
//
// UdpSocket::UdpSocket() : Socket(AF_INET, SOCK_DGRAM, 0) {}
//
// UdpSocket::~UdpSocket() = default;
//
// auto UdpSocket::set_destination(const std::string& host, uint16_t port)
//   -> void {
//   struct sockaddr_in address;
//   address.sin_family = AF_INET;
//   address.sin_port = htons(port);
//   inet_pton(AF_INET, host.c_str(), &address.sin_addr);
//
//   if (::connect(*socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
//     throw std::runtime_error("Could not connect socket");
//   }
// }
//
// auto UdpSocket::receive_from(size_t size)
//   -> std::pair<std::vector<uint8_t>, std::pair<std::string, uint16_t>> {
//   std::vector<uint8_t> buffer(size);
//   struct sockaddr_in sender_addr;
//   socklen_t addr_size = sizeof(sender_addr);
//
//   ssize_t received_bytes = recvfrom(get_socketfd(), buffer.data(), size, 0,
//                                     (struct sockaddr*)&sender_addr, &addr_size);
//   if (received_bytes == -1) {
//     // Handle error
//   }
//
//   std::string sender_ip(inet_ntoa(sender_addr.sin_addr));
//   uint16_t sender_port = ntohs(sender_addr.sin_port);
//
//   return {buffer, {sender_ip, sender_port}};
// }
//
// auto UdpSocket::send_to(const std::vector<uint8_t>& data,
//                         const std::string& host, uint16_t port) -> void {
//   struct sockaddr_in recipient_addr;
//   recipient_addr.sin_family = AF_INET;
//   recipient_addr.sin_port = htons(port);
//   inet_pton(AF_INET, host.c_str(), &recipient_addr.sin_addr);
//
//   if (sendto(get_socketfd(), data.data(), data.size(), 0,
//              (struct sockaddr*)&recipient_addr, sizeof(recipient_addr)) < 0) {
//     throw std::runtime_error("Could not send data");
//   }
// }
//
// }; // namespace networkssy::socket
