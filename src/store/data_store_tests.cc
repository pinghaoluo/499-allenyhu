#include "data_store.h"

#include <gtest/gtest.h>

TEST(DataStoreCreate, NewDataStore) {
  DataStore ds;
  ASSERT_NE(nullptr, &ds);
}

TEST(DataStorePut, BasePut) {
  DataStore ds;
  bool check = ds.Put("test", "test_value");
  EXPECT_EQ(true, check);
}

TEST(DataStorePut, PutSameKey) {
  DataStore ds;
  ds.Put("test", "1");
  ASSERT_EQ("1", ds.Get("test")[0]);
 
  bool check = ds.Put("test", "2");
  EXPECT_EQ(true, check);
  EXPECT_EQ("2", ds.Get("test")[1]);
}

TEST(DataStoreGet, BaseGet) {
  DataStore ds;
  ds.Put("test", "1");
  EXPECT_EQ("1", ds.Get("test")[0]);
}

TEST(DataStoreGet, GetNoKey) {
  DataStore ds;
  auto check = ds.Get("test");
  ASSERT_EQ(true, check.empty());
}

TEST(DataStoreDel, BaseDel) {
  DataStore ds;
  ds.Put("test", "1");
  
  EXPECT_EQ(true, ds.DeleteKey("test"));
  EXPECT_EQ(true, ds.Get("test").empty());
}

TEST(DataStoreDel, DelNoKey) {
  DataStore ds;
  EXPECT_EQ(false, ds.DeleteKey("test"));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
