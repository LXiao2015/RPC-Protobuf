#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>

#include "example.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

// final 关键字表示该类不会被继承
class SearchRequestImpl final : public SearchService::Service {
    Status Search(ServerContext* context, const SearchRequest* request, SearchResponse* reply) override {
        std::string prefix("Hello ");
        reply->set_response(prefix + request->request());
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:9001");
    SearchRequestImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main() {
    RunServer();
    return 0;
}
