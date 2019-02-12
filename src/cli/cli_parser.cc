#include "cli_parser.h"

DEFINE_string(registeruser, "", "username for registering a new user");
DEFINE_string(user, "", "username for user making command");
DEFINE_string(chirp, "", "text to be chirped");
DEFINE_string(reply, "", "reply id for given chirp");
DEFINE_string(follow, "", "user to follow");
DEFINE_string(read, "", "returns chirp thread starting with given chirp id");
DEFINE_bool(monitor, true, "starts monitoring for all following accounts' chirps");

CliParser::CliParser() : service_() {}

std::string CliParser::Parse(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  std::string s = "";
  if(!FLAGS_registeruser.empty()) {
    return ParseRegister(FLAGS_registeruser);
  }
  s += "user: ";
  s += FLAGS_user;
  return s;
}

std::string CliParser::ParseRegister(const std::string& uname) {
  if(service_.Register(uname)) {
    return uname + " successfully registered.";
  } 
  return uname + " failed to be registered.";
}
