#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using chrip::PutRequest;
using chirp::PutReply;
using chirp::KeyValueStore;

// grpc client to communicate with DataStoreServer
class DataStoreClient {
 public:
  // Default constructor for DataStoreClient
  DataStoreClient(std::shared_ptr<Channel> channel) : stub_(KeyValueStore::NewStub(channel)) {}

  // Put command to send PutRequest and receive PutReply
  // @key: key to be put
  // @val: val to be put at 'key'
  // @ret: true or false based on success
  bool Put(const std::string& key, const std::string& val) {
    PutRequest request;
    request.set_key(key);
    request.set_value(val);

    PutReply reply;
    ClientContext context;

    Status status = stub_->Put(&context, request, &reply);

    if(status.ok()) {
      return reply.message();
    } else {
      return false;
    }
  }

  // Get command to send GetRequest and receive GetReply
  // @key: key to be retrieved from
  // @ret: value retrieved from DataStore
  std::string Get(const std::string& key) {
    GetRequest request;
    request.set_key(key);

    GetReply reply;
    ClientContext context;

    Status status = stub_->Get(&context, request, &reply);

    if(status.ok()) {
      return reply.message();
    } else {
      return "";
    }
  }

  // DeleteKey command to send DeleteRequest and receive DeleteReply
  // @key: key to be deleted
  // @ret: ture or false based on success
  bool DeleteKey(const std::string& key) {
    GetRequest request;
    request.set_key(key);

    GetReply reply;
    ClientContext context;

    Status status = stub_->DeleteKey(&context, request, &reply);

    if(status.ok()) {
      return reply.message();
    } else {
      return false;
    }
  }

 private:
  std::unique_ptr<KeyValueStore::Stub> stub_;
};

// TODO do we need to make a main like in the grpc examples?
