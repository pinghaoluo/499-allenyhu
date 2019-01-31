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

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
