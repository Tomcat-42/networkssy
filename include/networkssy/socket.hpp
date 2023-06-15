#pragma once

#include <functional>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace networkssy::socket {

/*
 * @brief Protocol of the socket
 * @details TCP or UDP
 */
enum class Protocol : const int {
  TCP = SOCK_STREAM,
  UDP = SOCK_DGRAM,
};

/*
 * @brief Domain of the socket
 * @details IPv4, IPv6 or LOCAL. IPv4 and IPv6 are the most common
 * ones. And LOCAL is used for interprocess communication.
 */
enum class Domain : const int {
  IPV4 = AF_INET,
  IPV6 = AF_INET6,
  LOCAL = AF_LOCAL
};

/*
 * @brief An abstraction over the UNIX socket API.
 * @details This class is used by specific socket classes to
 * provide a common interface for the socket API.
 *
 * @see networkssy::socket::TcpSocket, networkssy::socket::UdpSocket and
 * networkssy::socket::LocalSocket
 *
 * @note This class is not thread-safe and is mainly blocking.
 */
class Socket {
protected:
  /*
   * @brief The socket domain.
   * @details This is used to specify the domain of the socket.
   *
   * @see networkssy::socket::Domain
   */
  Domain domain; // NOLINT

  /*
   * @brief The socket protocol.
   *
   * @details This is used to specify the protocol of the socket.
   *
   * @see networkssy::socket::Protocol
   */
  Protocol protocol; // NOLINT

  /*
   * @brief The socket file descriptor.
   * @details This is used to specify the file descriptor of the socket.
   *
   * @note This is a unique pointer to a const int, for RAII.
   */
  std::unique_ptr<const int, std::function<void(const int*)>> socket = // NOLINT
    nullptr;

  /*
   * @brief The underlying socket constructor.
   * @details This is used to specify the underlying function that
   * constructs the socket. The default is the UNIX socket API
   * socket(int, int, int).
   *
   * @see man 2 socket
   * @see man 7 socket
   */
  std::function<int(const int, const int, const int)>&&
    socket_constructor; // NOLINT

  /*
   * @brief The underlying socket destructor.
   *
   * @details This is used to specify the underlying function that
   * destructs the socket. The default is the UNIX socket API
   * close(int).
   *
   * @see man 2 close
   * @see man 7 socket
   */
  std::function<void(const int)>&& socket_destructor; // NOLINT

public:
  /*
   * @brief Constructs a socket.
   * @details This is used to construct a socket with the specified protocol
   * and domain.
   *
   * @param domain The domain of the socket.
   * @param protocol The protocol of the socket.
   * @param socket_constructor The underlying socket constructor. It defaults to
   * the UNIX socket API socket(int, int, int).
   * @param socket_destructor The underlying socket destructor. It defaults to
   * the UNIX socket API close(int).
   */
  Socket(Domain domain, Protocol protocol,
         std::function<int(const int, const int, const int)>&&
           socket_constructor = ::socket,
         std::function<void(const int)>&& socket_destructor = ::close);

  /*
   * @brief Destroys a socket.
   *
   * @details This is used to destroy a socket. It resets the socket unique
   * pointer, thus closing the socket.
   */
  ~Socket();

  /*
   * @brief Close a socket.
   *
   * @details This is used to close a socket. It resets the socket unique
   * pointer, thus closing the socket.
   */
  auto close() -> void;

  /*
   * @brief Get the socket file descriptor.
   *
   * @details This is used to get the socket file descriptor.
   *
   * @return The socket file descriptor.
   */
  [[nodiscard]] auto get_socketfd() const -> int;

  /*
   * @brief Set the socket file descriptor.
   *
   * @details This is used to set the socket file descriptor.
   *
   * @param new_socketfd The new socket file descriptor.
   */
  auto set_socketfd(int new_socketfd) -> void;
};

} // namespace networkssy::socket
