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
  if(!FLAGS_registeruser.empty()) {
    return ParseRegister(FLAGS_registeruser);
  }
  if(FLAGS_user.empty()) {
    return "Must be logged in to perform actions.";
  }
  
  std::string s = "";
  s += "user: "; 
  s += FLAGS_user + "\n";
  
  if(!FLAGS_chirp.empty()) {
    ParseChirp(FLAGS_user, FLAGS_chirp, FLAGS_reply);
  }
  return s;
}

std::string CliParser::ParseRegister(const std::string& uname) {
  if(service_.Register(uname)) {
    return uname + " successfully registered.";
  } 
  return uname + " failed to be registered.";
}

std::string CliParser::ParseChirp(const std::string& uname, const std::string& text,
		                  const std::string& reply_id) {
  auto id = reply_id.empty() ? std::nullopt : std::optional<std::string>{reply_id};
  Chirp c = service_.MakeChirp(uname, text, id); 
  if(c.username().empty()) {
    return "Chirp failed.";
  }
  if(reply_id.empty()) {
    return uname + " chirped: " + text;
  }
  return uname + " replied to " + reply_id + " with: " + text;
}
