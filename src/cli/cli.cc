#include <iostream>

#include <gflags/gflags.h>

#include "cli_parser.h"

int main(int argc, char** argv) {
  CliParser parser;
  std::cout << parser.Parse(argc, argv) << std::endl;
  return 0;
}
