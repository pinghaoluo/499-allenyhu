#include <sys/time.h>

// Wrapper class for Timestamp message
class TimeStamp {
 public:
  // Default constructor for Timestamp
  TimeStamp();

  // Constructor for Timestamp
  TimeStamp(int seconds, int useconds);
  
  // Getter for 'seconds_'
  inline const int& seconds() const {
    return seconds_;
  }

  // Getter for 'useconds_'
  inline const int& useconds() const {
    return useconds_;
  }

 private:
  int seconds_;
  int useconds_;
};
