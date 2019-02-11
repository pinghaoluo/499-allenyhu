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

// Tests Register method for ServiceLayer
// Registers with empty username, should fail
TEST(ServiceLayerRegister, RegisterEmpty) {
  ServiceLayer s;
  ASSERT_FALSE(s.Register(""));
}

// Tests MakeChirp method for ServiceLayer
// Makes a Chirp to add to DataStore, checks based on returned Chirp object
TEST(ServiceLayerMakeChirp, BaseMakeChirp) {
  ServiceLayer s;
  Chirp c = s.MakeChirp("test", "test text", "id");
  ASSERT_FALSE(c.id().empty());
  
  EXPECT_EQ("test", c.username());
  EXPECT_EQ("test text", c.text());
  EXPECT_EQ("id", c.parent_id());
}

// Tests MakeChirp method for ServiceLayer
// Makes a Chirp with no reply_id, Chirp object returned should not have a parent_id
TEST(ServiceLayerMakeChirp, MakeChirpNoReply) {
  ServiceLayer s;
  Chirp c = s.MakeChirp("test", "test text", std::nullopt);
  ASSERT_FALSE(c.id().empty());

  EXPECT_EQ("test", c.username());
  EXPECT_EQ("test text", c.text());
  EXPECT_TRUE(c.parent_id().empty());
}

// Tests Follow method for ServiceLayer
// `follower` should now follow `root`, based on bool return of Follow()
TEST(ServiceLayerFollow, BaseFollow) {
  ServiceLayer s;
  ASSERT_TRUE(s.Register("root"));
  ASSERT_TRUE(s.Register("follower"));

  EXPECT_TRUE(s.Follow("follower", "root"));
}

// Tests Follow method for ServiceLayer
// Attempts to follow a non-existent user, should return false
TEST(ServiceLayerFollow, FollowUnregisteredUser) {
  ServiceLayer s;
  ASSERT_TRUE(s.Register("follower"));

  EXPECT_FALSE(s.Follow("follower", "root"));
}

// Tests Follow method for ServiceLayer
// Attempts to call Follow with a non-existent user, should return false
TEST(ServiceLayerFollow, FollowByUnregisteredUser) {
  ServiceLayer s;
  ASSERT_TRUE(s.Register("root"));

  EXPECT_FALSE(s.Follow("follower", "root"));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
