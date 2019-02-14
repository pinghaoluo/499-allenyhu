#include <iostream>

#include "service_layer_client.h"

using chirp::Chirp;
using chirp::ChirpReply;
using chirp::ChirpRequest;
using chirp::FollowReply;
using chirp::FollowRequest;
using chirp::MonitorRequest;
using chirp::MonitorReply;
using chirp::ReadReply;
using chirp::ReadRequest;
using chirp::RegisterReply;
using chirp::RegisterRequest;
using chirp::ServiceLayer;
using chirp::Timestamp;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;

ServiceLayerClient::ServiceLayerClient(std::shared_ptr<Channel> channel) : stub_(ServiceLayer::NewStub(channel)) {}

bool ServiceLayerClient::Register(const std::string& uname) {
  RegisterRequest request;
  request.set_username(uname);

  RegisterReply reply;
  ClientContext context;

  Status status = stub_->registeruser(&context, request, &reply);
  return status.ok();
}

ChirpObj ServiceLayerClient::Chirp(const std::string& uname, const std::string& text, 
				const std::optional<std::string>& parent_id) {
  ChirpRequest request;
  request.set_username(uname);
  request.set_text(text);
  request.set_parent_id(parent_id.value_or(""));

  ChirpReply reply;
  ClientContext context;

  Status status = stub_->chirp(&context, request, &reply);
  if(status.ok()) {
    auto chirp = reply.chirp();
    return ChirpObj(chirp.username(), chirp.text(), chirp.id(),
		 chirp.parent_id(), chirp.timestamp().seconds(),
		 chirp.timestamp().useconds());
  } else {
    return ChirpObj();
  }
}

bool ServiceLayerClient::Follow(const std::string& uname, const std::string& to_follow_user) {
  FollowRequest request;
  request.set_username(uname);
  request.set_to_follow(to_follow_user);

  FollowReply reply;
  ClientContext context;

  Status status = stub_->follow(&context, request, &reply);
  return status.ok();
}

std::vector<ChirpObj> ServiceLayerClient::Read(const std::string& chirp_id) {
  ReadRequest request;
  request.set_chirp_id(chirp_id);

  ReadReply reply;
  ClientContext context;

  Status status = stub_->read(&context, request, &reply);
  std::vector<ChirpObj> replies;
  for(auto chirp : reply.chirps()) {
    ChirpObj o(chirp.username(), chirp.text(), chirp.id(),
             chirp.parent_id(), chirp.timestamp().seconds(),
             chirp.timestamp().useconds());
    replies.push_back(o);
  }
  return replies;
}

/**
int main(int argc, char** argv) {
  std::cout << "service client" << std::endl;
  ServiceLayerClient s(grpc::CreateChannel("0.0.0.0:50002",
		       grpc::InsecureChannelCredentials()));

  std::cout << s.Register("allen") << std::endl;
  std::cout << s.Chirp("allen", "allen says hi", std::nullopt).to_string() << std::endl;
  return 0;
}
*/
