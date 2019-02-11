#include "service_layer.h"

ServiceLayer::ServiceLayer() : ds_() {}

bool ServiceLayer::Register(const std::string& uname) {
  if(uname.empty() || !ds_.Get(uname).empty()) {
    return false;
  }
  return ds_.Put(uname, "registered");
}

Chirp ServiceLayer::MakeChirp(const std::string& uname, const std::string& text, const std::optional<std::string>& reply_id) {
  Chirp c(uname, text, reply_id);
  if(ds_.Put(c.id(), c.to_string())) {
    return c;
  }
  return Chirp();
}

bool ServiceLayer::Follow(const std::string& uname, const std::string& follow_uname) {
  if(ds_.Get(uname).empty() || ds_.Get(follow_uname).empty()) {
    return false;
  }
  return ds_.Put(uname, follow_uname);
}
