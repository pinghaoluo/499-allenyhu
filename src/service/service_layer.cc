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
    CheckMonitor(uname, c.to_string());
    return c;
  }
  return ChirpObj();
}

void ServiceLayer::MakeReply(const std::string& parent_id, const std::string& chirp_string) {
  std::string reply_key = parent_id + "-reply-";
  int counter = 0;
  std::string put_reply_key = reply_key + std::to_string(counter);
  while(!ds_.Get(put_reply_key).empty()) {
    counter++;
    put_reply_key = reply_key + std::to_string(counter);
  }
  ds_.Put(put_reply_key, chirp_string);
}

bool ServiceLayer::Follow(const std::string& uname, const std::string& follow_uname) {
  if(ds_.Get(uname).empty() || ds_.Get(follow_uname).empty()) {
    return false;
  }
  std::string monitor_key_base = uname + "-follow-";
  int counter = 0;
  std::string key = monitor_key_base + std::to_string(counter); 
  while(!ds_.Get(key).empty()) {
    counter++;
    key = monitor_key_base + std::to_string(counter);
  }
  return ds_.Put(key, follow_uname);
}

std::vector<ChirpObj> ServiceLayer::Read(const std::string& id) {
  std::vector<ChirpObj> replies;
  std::string chirp_id = id;
  auto chirp = ds_.Get(chirp_id);
  if(!chirp.empty()) {
    ChirpObj c = ParseChirpString(chirp[0]);
    replies.push_back(c);
    std::string reply_key_base = c.id() + "-reply-";
    ReadDfs(reply_key_base, &replies, 0);
  }
  return replies;
}

void ServiceLayer::ReadDfs(const std::string& key_base, std::vector<ChirpObj>* chirps, int counter) {
  auto chirp_string = ds_.Get(key_base + std::to_string(counter));

  //Base Case
  if(chirp_string.empty()) {
    return;
  }
  ChirpObj reply = ParseChirpString(chirp_string[0]);
  chirps->push_back(reply);
  
  // c has a reply
  std::string reply_key_base = reply.id() + "-reply-";
  ReadDfs(reply_key_base, chirps, 0);

  // another reply to same parent
  ReadDfs(key_base, chirps, counter+1);
}

std::vector<ChirpObj> ServiceLayer::Monitor(const std::string& uname) {
  std::vector<std::string> follows = GetFollows(uname);
  // Setting up DS to store monitor data
  for(const std::string& f : follows) {
    PutMonitorKey(uname, f); 
  }
  
  std::vector<ChirpObj> chirps;
  std::string monitor_check_base = uname + "-monitor-check-";
  int counter = 0;
  std::string key = monitor_check_base + std::to_string(counter);
  std::vector<std::string> check = ds_.Get(key);
  while(!check.empty()) {
    ChirpObj c = ParseChirpString(check[0]);
    chirps.push_back(c);
    ds_.DeleteKey(key);
    counter++;
    key = monitor_check_base + std::to_string(counter);
    check = ds_.Get(key);
  }
  
  return chirps;
}

void ServiceLayer::PutMonitorKey(const std::string& uname, const std::string& followed_user) {
  std::string monitor_key_base = followed_user + "-monitor-";
  int counter = 0;
  std::string key = monitor_key_base + std::to_string(counter);
  std::vector<std::string> check = ds_.Get(key);
  while(!check.empty()) {
    if(check[0] == uname) {
      // Already have bookkeeping entry
      return;
    }
    counter++;
    key = monitor_key_base + std::to_string(counter);
    check = ds_.Get(key);
  }

  // No matching monitor keys
  ds_.Put(key, uname);
}

void ServiceLayer::CheckMonitor(const std::string& uname, const std::string& chirp_string) {
  std::string monitor_key_base = uname + "-monitor-";
  int counter = 0;
  std::string key = monitor_key_base + std::to_string(counter);
  std::vector<std::string> check = ds_.Get(key);
  
  while(!check.empty()) {
    std::string monitorer = check[0];
    UpdateMonitor(monitorer, chirp_string);
    counter++;
    key = monitor_key_base + std::to_string(counter);
    check = ds_.Get(key);
  }
}

void ServiceLayer::UpdateMonitor(const std::string& uname, const std::string& chirp_string) {
  std::string monitor_check_base = uname + "-monitor-check-";
  int counter = 0;
  std::string key = monitor_check_base + std::to_string(counter);
  std::vector<std::string> check = ds_.Get(key); 
  
  while(!check.empty()) {
    counter++;
    key = monitor_check_base + std::to_string(counter);
    check = ds_.Get(key);
  }
  ds_.Put(key, chirp_string);
}

std::vector<std::string> ServiceLayer::GetFollows(const std::string& uname) {
  std::vector<std::string> followers;
  std::string follow_key_base = uname + "-follow-";
  int counter = 0;
  std::string key = follow_key_base + std::to_string(counter);
  std::vector<std::string> check = ds_.Get(key);
  
  while(!check.empty()) {
    followers.push_back(check[0]);
    counter++;
    key = follow_key_base + std::to_string(counter);
    check = ds_.Get(key);
  }
  
  return followers;
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
  
  return ChirpObj(data[2], text, id, parent_id, std::stoi(data[0]), std::stoi(data[1]));
}
