#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>

#include "example.grpc.pb.h"

using grpc::Server;    // gRPC server 类
using grpc::ServerBuilder;    // 构造和启动 gRPC server
using grpc::ServerContext;    // 为 RPC 传递上下文
using grpc::Status;    // 记录执行是否成功

// final 关键字表示该类不会被继承
class SearchRequestImpl final : public SearchService::Service {
    Status Search(ServerContext* context, const SearchRequest* request, SearchResponse* reply) override {    // 服务实现类
        std::string prefix("Hello ");
        reply->set_response(prefix + request->request());
        return Status::OK;    // 告知 gRPC 我们已经完成了响应的写入
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:9001");
    SearchRequestImpl service;    // 创建服务实现类 SearchRequestImpl 的一个实例

    ServerBuilder builder;    // 创建工厂类 ServerBuilder 的一个实例
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());    // 指定监听的地址和端口, 建立不安全的服务器凭据
    builder.RegisterService(&service);    // 注册服务实例
    std::unique_ptr<Server> server(builder.BuildAndStart());    // 创建和启动一个 gRPC server
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();    // 这个函数只有在服务器关闭或被其他线程调用 Shutdown 关闭时才返回
}

int main() {
    RunServer();
    return 0;
}
