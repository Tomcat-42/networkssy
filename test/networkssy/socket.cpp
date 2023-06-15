#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <networkssy/error.hpp>
#include <networkssy/socket.hpp>

using networkssy::socket::Domain;
using networkssy::socket::Protocol;
using networkssy::socket::Socket;
using testing::TestWithParam;
using testing::Values;

static constexpr const int VALID_SOCKETFD = 10;
static constexpr const int INVALID_SOCKETFD = -1;
static constexpr const int NEW_SOCKETFD = 20;

class SocketTest : public TestWithParam<std::tuple<Domain, Protocol>> {
protected:
  SocketTest()
    : domain(std::get<0>(GetParam())), protocol(std::get<1>(GetParam())) {}

  Domain domain;     // NOLINT
  Protocol protocol; // NOLINT
};

TEST_P(SocketTest, ConstructSocketSuccess) { // NOLINT
  auto socket_constructor_mock = [this](const int domain, const int protocol,
                                        const int) {
    EXPECT_EQ(domain, static_cast<int>(this->domain));
    EXPECT_EQ(protocol, static_cast<int>(this->protocol));
    return VALID_SOCKETFD;
  };

  auto socket_destructor_mock = [](const int socket_fd) {
    EXPECT_EQ(socket_fd, VALID_SOCKETFD);
  };

  Socket socket(domain, protocol, socket_constructor_mock,
                socket_destructor_mock);
  EXPECT_EQ(socket.get_socketfd(), VALID_SOCKETFD);
}

TEST_P(SocketTest, ConstructSocketFailure) { // NOLINT
  auto socket_constructor_mock = [](const int, const int, const int) {
    errno = EACCES;
    return INVALID_SOCKETFD;
  };

  auto socket_destructor_mock = [](const int) {
    FAIL();
  };

  try {
    Socket socket(domain, protocol, socket_constructor_mock,
                  socket_destructor_mock);
    FAIL();
  } catch (const std::runtime_error& e) {
    EXPECT_STREQ(e.what(), networkssy::error::ERROR_MAP.at(errno).c_str());
  }
}

TEST_P(SocketTest, SetSocketfd) { // NOLINT
  auto socket_constructor_mock = [](const int, const int, const int) {
    return VALID_SOCKETFD;
  };

  auto socket_destructor_mock = [](const int socket_fd) {
    EXPECT_TRUE(socket_fd == VALID_SOCKETFD || socket_fd == NEW_SOCKETFD);
  };

  Socket socket(domain, protocol, socket_constructor_mock,
                socket_destructor_mock);
  socket.set_socketfd(NEW_SOCKETFD);
  EXPECT_EQ(socket.get_socketfd(), NEW_SOCKETFD);
}

TEST_P(SocketTest, SetInvalidSocketfd) { // NOLINT
  auto socket_constructor_mock = [](const int, const int, const int) {
    return VALID_SOCKETFD;
  };

  auto socket_destructor_mock = [](const int socket_fd) {
    EXPECT_EQ(socket_fd, VALID_SOCKETFD);
  };

  Socket socket(domain, protocol, socket_constructor_mock,
                socket_destructor_mock);
  ASSERT_THROW(socket.set_socketfd(INVALID_SOCKETFD), std::runtime_error);
}

TEST_P(SocketTest, CloseSocket) { // NOLINT
  auto socket_constructor_mock = [](const int, const int, const int) {
    return VALID_SOCKETFD;
  };

  auto socket_destructor_mock = [](const int socket_fd) {
    EXPECT_EQ(socket_fd, VALID_SOCKETFD);
  };

  Socket socket(domain, protocol, socket_constructor_mock,
                socket_destructor_mock);
  socket.close();
  ASSERT_THROW(static_cast<void>(socket.get_socketfd()), std::runtime_error);
}

INSTANTIATE_TEST_SUITE_P(DomainAndProtocolTest, SocketTest, // NOLINT
                         Values(std::make_tuple(Domain::IPV4, Protocol::TCP),
                                std::make_tuple(Domain::IPV4, Protocol::UDP),
                                std::make_tuple(Domain::IPV6, Protocol::TCP),
                                std::make_tuple(Domain::IPV6, Protocol::UDP),
                                std::make_tuple(Domain::UNIX, Protocol::TCP),
                                std::make_tuple(Domain::UNIX, Protocol::UDP)));
