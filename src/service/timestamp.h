#include <sys/time.h>

// Wrapper class for Timestamp message
class Timestamp {
 public:
  // Default constructor for Timestamp
  Timestamp();
  
  // Getter for 'seconds_'
  const int& seconds() const;

  // Getter for 'useconds_'
  const int& useconds() const;

 private:
  int seconds_;
  int useconds_;
};
