#include "service_layer_grpc.h"

ServiceLayerObj::ServiceLayerObj() : ds_(grpc::CreateChannel("0.0.0.0:50000",
			           grpc::InsecureChannelCredentials())) {}

bool ServiceLayerObj::Register(const std::string& uname) {
  if(uname.empty() || !ds_.Get(uname).empty()) {
    return false;
  }
  return ds_.Put(uname, "registered");
}

ChirpObj ServiceLayerObj::MakeChirp(const std::string& uname, const std::string& text, const std::optional<std::string>& reply_id) {
  if(ds_.Get(uname).empty()) {
    return ChirpObj();
  }

  ChirpObj c(uname, text, reply_id);
  if(ds_.Put(c.id(), c.to_string())) {
    return c;
  }
  return ChirpObj();
}

bool ServiceLayerObj::Follow(const std::string& uname, const std::string& follow_uname) {
  if(ds_.Get(uname).empty() || ds_.Get(follow_uname).empty()) {
    return false;
  }
  return ds_.Put(uname, follow_uname);
}

/**
int main(int argc, char** argv) {
  ServiceLayerObj s;

  if(s.Register("allen")) {
    std::cout << "register SUCCESS" << std::endl;
  } else {
    std::cout << "register FAILURE" << std::endl;
  }

  return 0;
}
*/
