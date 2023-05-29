#include <gtest/gtest.h>

class serverssy_test : public ::testing::Test {
protected:
  const std::string expected = "serverssy";

  void SetUp() override {}
};

TEST_F(serverssy_test, serverssy) {
  // Arrange
  const std::string actual = "serverssy";

  // Act
  const bool result = (actual == expected);

  // Expect
  EXPECT_TRUE(result);
}
