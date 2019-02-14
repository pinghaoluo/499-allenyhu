#ifndef SRC_CLI_CLI_PARSER_H_
#define SRC_CLI_CLI_PARSER_H_

#include <string>
#include <unistd.h>

#include <gflags/gflags.h>

#include "../service/service_layer_client.h"

// GFLAG declarations for CLI args
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
  // ServiceLayerClient that connects CLI to the ServiceLayerServer
  ServiceLayerClient service_;

  // Helper function to parse register command
  std::string ParseRegister(const std::string& uname);

  // Helper function to parse chirp command
  std::string ParseChirp(const std::string& uname, const std::string& text, const std::string& reply_id);

  // Helper function to parse follow command
  std::string ParseFollow(const std::string& uname, const std::string& to_follow_user);
  
  // Helper function to parse read command
  std::string ParseRead(const std::string& chirp_id);
  
  // Helper function to parse monitor command
  std::string ParseMonitor(const std::string& uname);
};

#endif //SRC_CLI_CLI_PARSER_H_
