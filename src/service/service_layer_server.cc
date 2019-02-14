#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "service_layer.grpc.pb.h"
#include "service_layer_grpc.h"

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
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

class ServiceLayerServiceImpl final : public ServiceLayer::Service {
 public:
  explicit ServiceLayerServiceImpl() : service_() {}
  
  Status registeruser(ServerContext* context, const RegisterRequest* request, RegisterReply* response) override {
    bool status = service_.Register(request->username());
    if(status) {
      return Status::OK;
    }
    return Status::CANCELLED;
  }

  Status chirp(ServerContext* context, const ChirpRequest* request, ChirpReply* response) override {
    std::optional<std::string> reply_id = 
	    request->parent_id().empty() ? std::nullopt : std::optional<std::string>{request->parent_id()};
    
    ChirpObj c = service_.MakeChirp(request->username(), request->text(), reply_id);
    if(c.username().empty()) {
      return Status::CANCELLED;
    }

    Chirp* reply = response->mutable_chirp();
    reply->set_username(c.username());
    reply->set_text(c.text());
    reply->set_id(c.id());
    reply->set_parent_id(c.parent_id());
    reply->mutable_timestamp()->set_seconds(c.time().seconds());
    reply->mutable_timestamp()->set_useconds(c.time().useconds());

    return Status::OK;
  }
  
  Status follow(ServerContext* context, const FollowRequest* request, FollowReply* response) override {
    bool status = service_.Follow(request->username(), request->to_follow());
    
    if(status) {
      return Status::OK;
    }
    
    return Status::CANCELLED;
  }

  Status read(ServerContext* context, const ReadRequest* request, ReadReply* response) override {
    std::vector<ChirpObj> reply_chirps = service_.Read(request->chirp_id());
    for(auto c : reply_chirps) {
      Chirp* reply = response->add_chirps();
      reply->set_username(c.username());
      reply->set_text(c.text());
      reply->set_id(c.id());
      reply->set_parent_id(c.parent_id());
      reply->mutable_timestamp()->set_seconds(c.time().seconds());
      reply->mutable_timestamp()->set_useconds(c.time().useconds());
    }
    
    return Status::OK;
  }

  Status monitor(ServerContext* context, const MonitorRequest* request, ServerWriter<MonitorReply>* writer) override {
    std::vector<ChirpObj> chirps = service_.Monitor(request->username());
    for(const ChirpObj& c : chirps) {
      MonitorReply reply;
      Chirp* reply_chirp = reply.mutable_chirp();
      reply_chirp->set_username(c.username());
      reply_chirp->set_text(c.text());
      reply_chirp->set_id(c.id());
      reply_chirp->set_parent_id(c.parent_id());
      reply_chirp->mutable_timestamp()->set_seconds(c.time().seconds());
      reply_chirp->mutable_timestamp()->set_useconds(c.time().useconds()); 

      writer->Write(reply);
    }
    
    return Status::OK;
  }

 private:
  ServiceLayerObj service_;
};

void RunServer() {
  std::string server_address("0.0.0.0:50002");
  ServiceLayerServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Service Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}
