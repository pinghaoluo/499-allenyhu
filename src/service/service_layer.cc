#include "service_layer.h"

ServiceLayer::ServiceLayer() : ds_() {}

bool ServiceLayer::Register(const std::string& uname) {
  if(uname.empty() || !ds_.Get(uname).empty()) {
    return false;
  }
  return ds_.Put(uname, "registered");
}

Chirp::Chirp ServiceLayer::MakeChirp(const std::string& uname, const std::string& text, const std::optional<std::string>& reply_id) {

}
