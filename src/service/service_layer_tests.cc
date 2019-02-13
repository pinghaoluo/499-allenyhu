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
  s.Register("test");
  ChirpObj c = s.MakeChirp("test", "test text", "id");
  ASSERT_FALSE(c.id().empty());
  
  EXPECT_EQ("test", c.username());
  EXPECT_EQ("test text", c.text());
  EXPECT_EQ("id", c.parent_id());
}

// Tests MakeChirp method for ServiceLayer
// Makes a Chirp with no reply_id, Chirp object returned should not have a parent_id
TEST(ServiceLayerMakeChirp, MakeChirpNoReply) {
  ServiceLayer s;
  s.Register("test");
  ChirpObj c = s.MakeChirp("test", "test text", std::nullopt);
  ASSERT_FALSE(c.id().empty());

  EXPECT_EQ("test", c.username());
  EXPECT_EQ("test text", c.text());
  EXPECT_TRUE(c.parent_id().empty());
}

// Tests MakeChirp method for ServiceLayer
// Makes a Chirp with unregistered user. Should fail 
TEST(ServiceLayerMakeChirp, MakeChirpUnregisteredUser) {
  ServiceLayer s;
  ChirpObj c = s.MakeChirp("test", "test text", std::nullopt);
  EXPECT_TRUE(c.id().empty());
  EXPECT_TRUE(c.text().empty());
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

// Tests Read method for ServiceLayer
// Makes 2 Chirps, one in response to the other. Reads the original Chirp.
// Should return vector of Chirps in chrono order
TEST(ServiceLayerRead, BaseRead) {
  ServiceLayer s;
  s.Register("c1");
  s.Register("c2");

  ChirpObj c1 = s.MakeChirp("c1", "c1 text", std::nullopt);
  ASSERT_FALSE(c1.id().empty());
  std::string c1_id = c1.id();

  ChirpObj c2 = s.MakeChirp("c2", "c2 text", c1_id);
  ASSERT_FALSE(c2.id().empty());
  std::string c2_id = c2.id();

  auto replies = s.Read(c1_id);
  ASSERT_EQ(2, replies.size());
  EXPECT_EQ(c1_id, replies[0]);
  EXPECT_EQ(c2_id, replies[1]); 
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
