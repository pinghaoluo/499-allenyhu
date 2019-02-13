#include "service_layer.h"

ServiceLayer::ServiceLayer() : ds_() {}

bool ServiceLayer::Register(const std::string& uname) {
  if(uname.empty() || !ds_.Get(uname).empty()) {
    return false;
  }
  return ds_.Put(uname, "registered");
}

ChirpObj ServiceLayer::MakeChirp(const std::string& uname, const std::string& text, 
		                 const std::optional<std::string>& reply_id) {
  if(ds_.Get(uname).empty()) {
    return ChirpObj();
  }

  ChirpObj c(uname, text, reply_id);
  if(ds_.Put(c.id(), c.to_string())) {
    if(!c.parent_id().empty()) {
      MakeReply(c.parent_id(), c.to_string());
    }
    return c;
  }
  return ChirpObj();
}

void ServiceLayer::MakeReply(const std::string& parent_id, const std::string& chirp_string) {
  std::string reply_key = parent_id + "-reply-";
  int counter = 0;
  std::string put_reply_key = reply_key + std::to_string(counter);
  while(!ds_.Put(put_reply_key, chirp_string)) {
    counter++;
    put_reply_key = reply_key + std::to_string(counter);
  }
}

bool ServiceLayer::Follow(const std::string& uname, const std::string& follow_uname) {
  if(ds_.Get(uname).empty() || ds_.Get(follow_uname).empty()) {
    return false;
  }
  return ds_.Put(uname, follow_uname);
}

std::vector<std::string> ServiceLayer::Read(const std::string& id) {
  std::vector<std::string> replies;
  std::string chirp_id = id;
  auto chirp = ds_.Get(chirp_id);
  int counter = 0;
  while(!chirp.empty()) {
    ChirpObj c = ParseChirpString(chirp[0]);
    replies.push_back(c.id());
    std::string reply_id = c.id() + "-reply-" + std::to_string(counter);
    chirp = ds_.Get(reply_id);
    counter++;
  }
  return replies;
}

ChirpObj ServiceLayer::ParseChirpString(const std::string& chirp) {
  std::string s;
  std::size_t id_pos = chirp.find("|");
  int id_len = std::stoi(chirp.substr(0, id_pos));
  std::string id = chirp.substr(id_pos+1, id_len);
  s = chirp.substr(id_pos+id_len+1);
  
  std::size_t text_pos = s.find("|");
  int text_len = std::stoi(s.substr(0, text_pos));
  std::string text = s.substr(text_pos+1, text_len);
  s = s.substr(text_pos+text_len+1);

  std::string parent_id = "";
  if(!s.empty()) {
    std::size_t parent_pos = s.find("|");
    int parent_len = std::stoi(s.substr(0, parent_pos));
    parent_id = s.substr(parent_pos+1, parent_len);
  }

  //Parse for timestamp and username from id
  std::vector<std::string> data;
  std::stringstream ss(id);
  std::string token;
  while(std::getline(ss, token, '-')) {
    data.push_back(token);
  }
  //data is now {seconds, useconds, username}
  
  return ChirpObj(data[2], text, parent_id, id, std::stoi(data[0]), std::stoi(data[1]));
}
