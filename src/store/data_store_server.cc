#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "data_store.grpc.pb.h"
#include "data_store.h"

using chirp::DeleteRequest;
using chirp::DeleteReply;
using chirp::GetRequest;
using chirp::GetReply;
using chirp::KeyValueStore;
using chirp::PutRequest;
using chirp::PutReply;
using grpc::Channel;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;

class KeyValueStoreServiceImpl final : public KeyValueStore::Service {
 public:
  // Assumes only one KeyValueStoreServiceImpl will be active at one time
  explicit KeyValueStoreServiceImpl() : store_() {}
	  
  Status put(ServerContext* context, const PutRequest* request, PutReply* response) override {
    bool status = store_.Put(request->key(), request->value());
    if (status) {
      return Status::OK;
    }
    return Status::CANCELLED;
  }

  Status get(ServerContext* context, 
	     ServerReaderWriter<GetReply, GetRequest>* stream) override {
    std::vector<GetRequest> received_requests;
    GetRequest req;
    
    while (stream->Read(&req)) {
      std::vector<std::string> vals = store_.Get(req.key());
      for (const std::string& s : vals) {
        GetReply reply;
	reply.set_value(s);
	stream->Write(reply);
      }
    }
    
    return Status::OK;
  }

  Status deletekey(ServerContext* context, const DeleteRequest* request, DeleteReply* response) override {
    if(store_.DeleteKey(request->key())) {
      return Status::OK;
    }
    
    return Status::CANCELLED;
  }
 
 private:
  // Actual DataStore object to be communicating with
  DataStore store_;
};

void RunServer() {
  std::string server_address("0.0.0.0:50000");
  KeyValueStoreServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "KeyValue Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}
