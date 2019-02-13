#include "data_store_client.h"

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

DataStoreClient::DataStoreClient(std::shared_ptr<Channel> channel) : stub_(KeyValueStore::NewStub(channel)) {}

bool DataStoreClient::Put(const std::string& key, const std::string& val) {
  PutRequest request;
  request.set_key(key);
  request.set_value(val);

  PutReply reply;
  ClientContext context;
    
  std::cout << "Server Put: " << key << " | " << val << std::endl;
  Status status = stub_->put(&context, request, &reply);
  return status.ok();
}

std::vector<std::string> DataStoreClient::Get(const std::string& key) {
  ClientContext context;
    
  std::shared_ptr<ClientReaderWriter<GetRequest, GetReply> > stream(
    stub_->get(&context));
    
  std::thread writer([stream, key]() {
    std::cout << "GetRequest: " << key << std::endl;
    if(stream->Write(MakeGetRequest(key))) {
      std::cout << "success write from client" << std::endl;
    } else {
      std::cout << "client write failed" << std::endl;
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

bool DataStoreClient::DeleteKey(const std::string& key) {
  DeleteRequest request;
  request.set_key(key);

  DeleteReply reply;
  ClientContext context;

  Status status = stub_->deletekey(&context, request, &reply);
  return status.ok();
}

int main(int argc, char** argv) {
 DataStoreClient client(grpc::CreateChannel("0.0.0.0:50000",
			grpc::InsecureChannelCredentials()));
 
 client.Put("test", "test text");
 std::vector<std::string> result = client.Get("test");
 std::cout << result[0] << std::endl;
 return 0; 
}

