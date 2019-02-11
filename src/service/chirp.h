#include <optional>
#include <string>

#include "timestamp.h"

// Wrapper object for Chirp message
class Chirp {
 public:
  // Default constructor for Chirp object, used on failure
  Chirp();

  // Constructor for Chirp object
  Chirp(const std::string& uname, const std::string& text, const std::optional<std::string>& parent_id);
  
  // Getter for `username_`
  const std::string& username() const;

  // Getter for `text_`
  const std::string& text() const;

  // Getter for `id_`
  const std::string& id() const;

  // Getter for `parent_id`
  const std::string& parent_id() const;

  // Getter for `time_`
  const Timestamp& time() const;

  // Helper method to convert Chirp into a string for GRPC transfer
  std::string to_string();

 private:
   std::string username_;
   std::string text_;
   std::string id_;
   std::string parent_id_;
   Timestamp time_;
};
