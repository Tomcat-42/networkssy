#include <cxxopts.hpp>
#include <iostream>
#include <networkssy/networkssy.hpp>

auto main(int argc, char* argv[]) -> int {
  cxxopts::Options options("serverssy",
                           "Networkssy server for receiving files.");

  options.add_options()("p,protocol",
                        "Protocol to use (tcp or udp), default is tcp",
                        cxxopts::value<std::string>()->default_value("tcp"))(
    "g,guarantee", "Guarantee delivery if UDP is used (default is no)",
    cxxopts::value<bool>()->default_value("false"))(
    "b,block", "Block size, default is 1000 bytes",
    cxxopts::value<int>()->default_value("1000"))(
    "port", "Server port", cxxopts::value<uint16_t>())("h,help", "Print help");

  options.parse_positional({"port"});

  try {
    auto result = options.parse(argc, argv);

    if (result.count("help") != 0U) {
      std::cout << options.help() << std::endl;
      return 0;
    }

    std::string protocol = result["protocol"].as<std::string>();
    bool guarantee = result["guarantee"].as<bool>();
    int block_size = result["block"].as<int>();
    uint16_t port = result["port"].as<uint16_t>();

    std::cout << protocol << std::endl;

    std::unique_ptr<networkssy::connection> conn;
    if (protocol == "tcp") {
      conn = std::make_unique<networkssy::tcp_connection>();
    } else if (protocol == "udp") {
      conn = std::make_unique<networkssy::udp_connection>();
    } else {
      std::cerr << "Invalid protocol. Use tcp or udp." << std::endl;
      return 1;
    }

    std::cout << "Server is starting on port " << port << std::endl;
    conn->start_server(port);
    std::cout << "Server started" << std::endl;

    conn->accept_client();
    std::cout << "Client accepted" << std::endl;

    while (true) {
      std::vector<uint8_t> buffer;
      if (guarantee && protocol == "udp") {
        auto received = dynamic_cast<networkssy::udp_connection*>(conn.get())
                          ->receive_with_ack(block_size);
        buffer = std::move(received.first);
      } else {
        std::cout << "Receiving..." << std::endl;
        buffer = conn->receive(block_size);
      }

      if (buffer.empty()) {
        break;
      }

      std::cout.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    }

    conn->disconnect();
  } catch (const cxxopts::exceptions::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << options.help() << std::endl;
    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
