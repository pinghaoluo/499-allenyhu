#include <sys/time.h>

// Wrapper class for Timestamp message
class TimeStamp {
 public:
  // Default constructor for Timestamp
  TimeStamp();

  // Constructor for Timestamp
  TimeStamp(int seconds, int useconds);
  
  // Getter for 'seconds_'
  const int& seconds() const;

  // Getter for 'useconds_'
  const int& useconds() const;

 private:
  int seconds_;
  int useconds_;
};
