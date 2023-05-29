#include <gtest/gtest.h>

class clientssy_test : public ::testing::Test {
protected:
  const std::string expected = "clientssy";

  void SetUp() override {}
};

TEST_F(clientssy_test, clientssy) {
  // Arrange
  const std::string actual = "clientssy";

  // Act
  const bool result = (actual == expected);

  // Expect
  EXPECT_TRUE(result);
}
