#include "data_store.h"

DataStore::DataStore() : table_(), lock_() {}

bool DataStore::Put(const std::string& key, const std::string& val) {
  std::lock_guard<std::mutex> lg(lock_);
  std::pair<std::string, std::string> key_val(key, val);
  auto check = table_.insert(key_val);
  return check.second;
}

std::string DataStore::Get(const std::string& key) {
  std::lock_guard<std::mutex> lg(lock_);
  auto val = table_.find(key);
  if(val == table_.end()) {
    return ""; //empty string to represnt no value
  }
  return val->second;
}

bool DataStore::DeleteKey(const std::string& key) {
  std::lock_guard<std::mutex> lg(lock_);
  int check = table_.erase(key);
  if(check) {
    return true;
  }
  return false;
}
