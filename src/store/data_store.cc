#include <string>
#include <unordered_map>

#include <grpc/grpc.h>

//Mapping out data store for Chirp

class DataStore {
 public:
  DataStore();
  bool put(std::string key, std::string val);
  std::string get(std::string key);
  bool delete(std::string key);
 
 private:
  std::unordered_map<std::string, std::string> table;
};
