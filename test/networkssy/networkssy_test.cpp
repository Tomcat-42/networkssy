#include <gtest/gtest.h>
#include <shared_lib/shared_lib.hpp>

// shared_lib_test fixture
class shared_lib_test : public ::testing::Test {
protected:
  const std::string expected = "shared_lib";

  void SetUp() override {}
};

TEST_F(shared_lib_test, name) {
  // Arrange
  const std::string actual = shared_lib::shared_lib();

  // Act
  const bool result = (actual == expected);

  // Expect
  EXPECT_TRUE(result);
}
