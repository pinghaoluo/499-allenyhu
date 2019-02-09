#include "service_layer.h"

#include <gtest/gtest.h>

// Tests default constructor for ServiceLayer
TEST(ServiceLayerCreate, NewServiceLayer) {
  ServiceLayer s;
  ASSERT_NE(nullptr, &s);
}

// Tests Register method for ServiceLayer
// Registers a new user
TEST(ServiceLayerRegister, BaseRegister) {
  ServiceLayer s;
  ASSERT_TRUE(s.Register("root"));
}

// Tests Register method for ServiceLayer
// Registers same user twice, should fail on second attempt
TEST(ServiceLayerRegister, DoubleRegister) {
  ServiceLayer s;
  EXPECT_EQ(true, s.Register("root"));
  ASSERT_FALSE(s.Register("root"));
}

TEST(ServiceLayerRegister, RegisterEmpty) {
  ServiceLayer s;
  ASSERT_FALSE(s.Register(""));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
