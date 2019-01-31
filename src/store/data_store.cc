#include "data_store.h"

DataStore::DataStore() : table_(), lock_() {}

bool DataStore::Put(const std::string& key, const std::string& val) {
  std::lock_guard<std::mutex> lg(lock_);
  std::pair<std::string, std::string> key_val(key, val);
  auto check = table_.insert(key_val);
  return check.second;
}
