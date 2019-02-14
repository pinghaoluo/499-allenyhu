#ifndef SRC_SERVICE_CHIRP_OBJ_H_
#define SRC_SERVICE_CHIRP_OBJ_H_

#include <optional>
#include <string>

#include "timestamp.h" 

// Wrapper object for Chirp message
class ChirpObj {
 public:
  // Default constructor for Chirp object, used on failure
  ChirpObj();

  // Constructor for Chirp object when sending from user side
  ChirpObj(const std::string& uname, const std::string& text, const std::optional<std::string>& parent_id);

  // Constructor for Chirp object when receiving from server side
  ChirpObj(const std::string& uname, const std::string& text, const std::string& id,
             const std::string& parent_id, int seconds, int useconds);
  
  // Getter for `username_`
  const std::string& username() const;

  // Getter for `text_`
  const std::string& text() const;

  // Getter for `id_`
  const std::string& id() const;

  // Getter for `parent_id`
  const std::string& parent_id() const;

  // Getter for `time_`
  const TimeStamp& time() const;

  // Helper method to convert Chirp into a string for GRPC transfer
  std::string to_string();

  // Helper method to print Chirp data
  std::string print_string();

 private:
   std::string username_;
   std::string text_;
   std::string id_;
   std::string parent_id_;
   TimeStamp time_;
};

#endif // SRC_SERVICE_CHIRP_OBJ_H_
