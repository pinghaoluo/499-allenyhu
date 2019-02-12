#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>

#include "data_store.grpc.pb.h"

using chirp::DeleteRequest;
using chirp::DeleteReply;
using chirp::GetRequest;
using chirp::GetReply;
using chirp::KeyValueStore;
using chirp::PutRequest;
using chirp::PutReply;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;

// Helper method to make a GetRequest
GetRequest MakeGetRequest(const std::string& key) {
  GetRequest r;
  r.set_key(key);
  return r;
}

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

    Status status = stub_->put(&context, request, &reply);
    return status.ok();
  }

  // Get command to send GetRequest and receive GetReply
  // @key: key to be retrieved from
  // @ret: value retrieved from DataStore
  std::vector<std::string> Get(const std::vector<std::string>& keys) {
    ClientContext context;
    
    std::shared_ptr<ClientReaderWriter<GetRequest, GetReply> > stream(
      stub_->get(&context));
    
    std::thread writer([stream, keys]() {
      for(const std::string& s : keys) {
        std::cout << "GetRequest: " << s << std::endl;
        stream->Write(MakeGetRequest(s));
      }
      stream->WritesDone();
    });

    GetReply reply;
    std::vector<std::string> replies;
    while(stream->Read(&reply)) {
      std::cout << "GetReply: " << reply.value() << std::endl;
      replies.push_back(reply.value());
    }
    writer.join();
    Status status = stream->Finish();

    if(status.ok()) {
      return replies;
    } else {
      return std::vector<std::string>{};
    }
  }

  // DeleteKey command to send DeleteRequest and receive DeleteReply
  // @key: key to be deleted
  // @ret: ture or false based on success
  bool DeleteKey(const std::string& key) {
    DeleteRequest request;
    request.set_key(key);

    DeleteReply reply;
    ClientContext context;

    Status status = stub_->deletekey(&context, request, &reply);
    return status.ok();
  }

 private:
  std::unique_ptr<KeyValueStore::Stub> stub_;
};

int main(int argc, char** argv) {
 DataStoreClient client(grpc::CreateChannel("localhost:50000",
			grpc::InsecureChannelCredentials()));
 std::cout << client.Put("test", "test text") << std::endl;
 std::vector<std::string> v {"test"};
 std::cout << client.Get(v)[0] << std::endl;

 return 0; 
}
