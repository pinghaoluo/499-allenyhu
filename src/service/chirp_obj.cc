#include "chirp_obj.h"
#include <iostream>

ChirpObj::ChirpObj() : username_(""), text_(""), id_(""), parent_id_(""), time_() {}

ChirpObj::ChirpObj(const std::string& uname, const std::string& text, const std::optional<std::string>& parent_id) : 
             username_(uname), text_(text), time_() {
  parent_id_ = parent_id.value_or("");
  id_ = std::to_string(time_.seconds()) + "-" + std::to_string(time_.useconds()) + "-" + username_;
}

ChirpObj::ChirpObj(const std::string& uname, const std::string& text, const std::string& parent_id,
	     const std::string& id, int seconds, int useconds) 
	: username_(uname), text_(text), id_(id), parent_id_(parent_id), time_(seconds, useconds) {}

const std::string& ChirpObj::username() const {
  return username_;
}

const std::string& ChirpObj::text() const {
  return text_;
}

const std::string& ChirpObj::id() const {
  return id_;
}

const std::string& ChirpObj::parent_id() const {
  return parent_id_;
}

const TimeStamp& ChirpObj::time() const {
  return time_;
}

std::string ChirpObj::to_string() {
  std::string s = "";
  s += std::to_string(id_.length()) + "|" + id_;
  s += std::to_string(text_.length()) + "|" + text_;
  s += std::to_string(parent_id_.length()) +"|" + parent_id_;
  return s;
}

void ChirpObj::print() {
  std::cout << "chirp id: " << id_ << std::endl;
  std::cout << "time: " << time_.seconds() << "-" << time_.useconds() << std::endl;
  std::cout << "reply id: " << parent_id_ << std::endl;
  std::cout << "chirp text: " << text_ << std::endl;
}

// Timestamp defined in this file due to small size
// Helps with Makefile
TimeStamp::TimeStamp() {
  timeval t;
  gettimeofday(&t, nullptr);
  seconds_ = t.tv_sec;
  useconds_ = t.tv_usec;
}

TimeStamp::TimeStamp(int seconds, int useconds) : seconds_(seconds), useconds_(useconds) {}

const int& TimeStamp::seconds() const {
  return seconds_;
}

const int& TimeStamp::useconds() const{
  return useconds_;
}
/**
int main(int argc, char** argv) {
  ChirpObj c("test", "text", std::nullopt);
  c.print();
  std::cout << c.to_string() << std::endl;
  return 0;
}
*/
