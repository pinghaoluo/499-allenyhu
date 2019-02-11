#include "timestamp.h"

Timestamp::Timestamp() {
  timeval t;
  gettimeofday(&t, nullptr);
  seconds_ = t.tv_sec;
  useconds_ = t.tv_usec;
}

const int& Timestamp::seconds() const {
  return seconds_;
}

const int& Timestamp::useconds() const{
  return useconds_;
}
