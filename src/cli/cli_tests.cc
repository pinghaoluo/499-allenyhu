#include "cli_parser.h"

#include <gtest/gtest.h>

// Tests default constructor for CliParser
TEST(CliCreate, NewCliParser) {
  CliParser c;
  ASSERT_NE(nullptr, &c);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
