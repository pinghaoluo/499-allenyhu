#include <string>

#include <gflags/gflags.h>

#include "../service/service_layer.h"

DECLARE_string(user);
DECLARE_string(chirp);
DECLARE_string(reply);
DECLARE_string(follow);
DECLARE_string(read);
DECLARE_bool(monitor);

// Wrapper class to support CLI parsing
class CliParser {
 public:
  // Default constructor for CliParser
  CliParser(); 

  // Parses given commandline arguments
  std::string Parse(int argc, char** argv);
 
 private:
  ServiceLayer service_;

  // Helper function to parse register command
  std::string ParseRegister(const std::string& uname);
};
