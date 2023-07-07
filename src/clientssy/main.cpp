#include <cxxopts.hpp>
#include <iostream>
#include <networkssy/networkssy.hpp>

auto main(int argc, char* argv[]) -> int {
  cxxopts::Options options(
    "clientssy", "Networkssy client for sending and receiving files.");

  options.add_options()("p,protocol",
                        "Protocol to use (tcp or udp), default is tcp",
                        cxxopts::value<std::string>()->default_value("tcp"))(
    "g,guarantee", "Guarantee delivery if UDP is used (default is no)",
    cxxopts::value<bool>()->default_value("false"))(
    "b,block", "Block size, default is 1000 bytes",
    cxxopts::value<int>()->default_value("1000"))(
    "address", "Server address (format: ADDRESS:PORT)",
    cxxopts::value<std::string>())("h,help", "Print help");

  options.parse_positional({"address"});
  try {
    auto result = options.parse(argc, argv);

    if (result.count("help") != 0U) {
      std::cout << options.help() << std::endl;
      return 0;
    }

    std::string protocol = result["protocol"].as<std::string>();
    bool guarantee = result["guarantee"].as<bool>();
    int block_size = result["block"].as<int>();
    std::string address_port_str = result["address"].as<std::string>();

    size_t delimiter_pos = address_port_str.find(':');
    if (delimiter_pos == std::string::npos) {
      std::cerr << "Invalid address. Format: ADDRESS:PORT" << std::endl;
      return 1;
    }

    std::string address = address_port_str.substr(0, delimiter_pos);
    uint16_t port = std::stoul(address_port_str.substr(delimiter_pos + 1));

    std::unique_ptr<networkssy::connection> conn;
    if (protocol == "tcp") {
      conn = std::make_unique<networkssy::tcp_connection>();
    } else if (protocol == "udp") {
      conn = std::make_unique<networkssy::udp_connection>();
    } else {
      std::cerr << "Invalid protocol. Use tcp or udp." << std::endl;
      return 1;
    }

    std::cout << "Connecting to " << address << ":" << port << std::endl;
    conn->connect_to_server(address, port);
    std::cout << "Connected" << std::endl;

    std::vector<uint8_t> buffer(block_size);
    std::cout << "buffer size: " << block_size << std::endl;
    size_t number_of_retries = 0;
    while (true) {
      std::cin.read(reinterpret_cast<char*>(buffer.data()), block_size);
      std::streamsize size = std::cin.gcount();

      if (size == 0) { // No more data to read
        break;
      }

      printf("Read %lu bytes\n", static_cast<size_t>(size));
      buffer.resize(size);

      if (guarantee && protocol == "udp") {
        number_of_retries +=
          dynamic_cast<networkssy::udp_connection*>(conn.get())
            ->send_with_ack(buffer, address, port);
      } else {
        conn->send(buffer);
      }
    }

    if (guarantee && protocol == "udp") {
      std::cout << "Number of retries: " << number_of_retries << std::endl;
    }

    conn->disconnect();
  } catch (const cxxopts::exceptions::exception& e) {
    // print error and help
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << options.help() << std::endl;
    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
