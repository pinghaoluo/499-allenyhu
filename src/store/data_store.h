#include <mutex>
#include <string>
#include <unordered_map>

#include <grpc/grpc.h>

// Model class for data store component of Chirp system. Uses unordered_map to store data.
class DataStore {
 public:
  // Default constructor for DataStore class. Will initialize as empty table with lock
  DataStore();
  
  // Adds key value pair to table_
  // @key: key under which new `val` will be added
  // @val: the value to be added
  // @ret: true on successful insertion; false otherwise
  bool Put(const std::string& key, const std::string& val);

  // Returns string from requested key
  // @key: the key to be looked up
  // @ret: the value at `key`
  std::string Get(const std::string& key);

  // Deletes the value at given `key`
  // @key: the key of the key value pair to be removed
  // @ret: true on successful removal; false otherwise
  bool DeleteKey(const std::string& key);
 
 private:
  // Hashtable to store key value pairs
  std::unordered_map<std::string, std::string> table_;
  
  // Lock associated with `table_` to make threadsafe
  std::mutex lock_;
};
