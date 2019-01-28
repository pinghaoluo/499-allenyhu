#include "chirp.grpc.pb.h"

#include "data_store.h"

class KeyValueStoreServiceImpl final : public Chirp::Service {
 public:
  // Assumes only one KeyValueStoreServiceImpl will be active at one time
  explicit KeyValueStoreServiceImpl() : store_() {}
	  
  Status put(ServerContext* context, PutRequest* request) override {
    bool status = store_.put(request->key, request->value);
    if (status) {
      return Status::OK;
    }
    return Status::CANCELLED;
  }

  Status get(ServerContext* context, 
	     ServerReaderWriter(GetRequest, GetReply>* stream) override {
    std::vector<GetRequest> received_requests;
    GetRequest req;
    while (stream->Read(&req)) {
      for (const GetRequest& r : received_requests) {
       GetReply reply;
       reply.set_value(store_.get(r.key()));
       stream->Write(reply);
      }
      received_requests.push_back(req);
    }
    return Status::OK;
  }

  Status deletekey(ServerContext* context, DeleteRequest* request) override {
    store_.delete(request->key);
    
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:5000");
  KeyValueStoreServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsercureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "KeyValue Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}
