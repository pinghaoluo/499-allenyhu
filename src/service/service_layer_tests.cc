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

TEST(ServiceLayerMakeChirp, BaseMakeChirp) {
  ServiceLayer s;
  Chirp c = s.MakeChirp("test", "test text", "id");
  ASSERT_FALSE(c.id().empty());
  
  EXPECT_EQ("test", c.username());
  EXPECT_EQ("test text", c.text());
  EXPECT_EQ("id", c.parent_id());
}

TEST(ServiceLayerMakeChirp, MakeChirpNoReply) {
  ServiceLayer s;
  Chirp c = s.MakeChirp("test", "test text", nullptr);
  ASSERT_FALSE(c.id().empty());

  EXPECT_EQ("test", c.username());
  EXPECT_EQ("test text", c.text());
  EXPECT_TRUE(c.parent_id().empty());
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
