#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace networkssy {
/**
 * @brief Interface for network connections
 *
 * @details This class is an interface for network connections. It provides
 * methods to connect, disconnect, send and receive data.
 *
 */
class connection {
public:
  /**
   * @brief Connect to a host
   *
   * @details This method connects to a host.
   *
   * @param host The host to connect to
   * @param port The port to connect to
   */
  virtual void connect(const std::string& host, uint16_t port) = 0;

  /**
   * @brief Disconnect from the host
   *
   * @details This method disconnects from the host.
   *
   */
  virtual void disconnect() = 0;

  /**
   * @brief Send data to the host
   *
   * @details This method sends data to the host.
   *
   * @param data The data to send
   */
  virtual void send(const std::vector<uint8_t>& data) = 0;

  /**
   * @brief Receive data from the host
   *
   * @details This method receives data from the host.
   *
   * @param size The size of the data to receive
   * @return The received data
   */
  virtual auto receive(size_t size) -> std::vector<uint8_t> = 0;
};

class socket {
public:
  socket(int domain, int type, int protocol);
  ~socket();

  auto bind(const std::string& host, uint16_t port) const -> void;
  auto close() const -> void;

private:
  int socket_fd;
};
} // namespace networkssy
