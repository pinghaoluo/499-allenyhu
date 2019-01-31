#include "data_store.h"

DataStore::DataStore() : table_(), lock_() {}

bool DataStore::Put(const std::string& key, const std::string& val) {
  std::lock_guard<std::mutex> lg(lock_);

  if(table_.find(key) != table_.end()) {
    table_.at(key).push_back(val);
    return true;
  } else {
    std::vector<std::string> v = {val};
    std::pair<std::string, std::vector<std::string> > key_val(key, v);
    auto check = table_.insert(key_val);
    return check.second;
  }
  return false;
}

std::vector<std::string> DataStore::Get(const std::string& key) {
  std::lock_guard<std::mutex> lg(lock_);
  auto val = table_.find(key);
  if(val == table_.end()) {
    std::vector<std::string> v;
    return v; //return empty vector
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
