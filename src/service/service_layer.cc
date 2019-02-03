#include "service_layer.h"

ServiceLayer::ServiceLayer() : ds_() {}

bool ServiceLayer::Register(const std::string& uname) {
  return ds_.Put(uname, "registered");
}
