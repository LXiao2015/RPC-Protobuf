#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

#include "example.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

class ExampleClient {
public:
    explicit ExampleClient(std::shared_ptr<Channel> channel) : stub_(SearchService::NewStub(channel)) {}
    std::string Search(const std::string& user) {
        SearchRequest request;
        request.set_request(user);

        ClientContext context;

        // 初始化 RPC 并为之创建句柄, 将 RPC 绑定到一个 CompletionQueue
        CompletionQueue cq;
        std::unique_ptr<ClientAsyncResponseReader<SearchResponse> > rpc(stub_->AsyncSearch(&context, request, &cq));

        Status status;
        SearchResponse reply;
        
        // 用一个唯一的标签，寻求回答和最终的状态
        rpc->Finish(&reply, &status, (void*)1);
        
        // 等待完成队列返回下一个标签, 当标签被传入对应的 Finish() 调用时, 回答和状态就可以被返回了
        void* got_tag;
        bool ok = false;
        GPR_ASSERT(cq.Next(&got_tag, &ok));
        
        GPR_ASSERT(got_tag == (void*)1);
        GPR_ASSERT(ok);

        if (status.ok()) {
            return reply.response();
        } else {
            return "RPC DAILED";
        }
    }

private:
    std::unique_ptr<SearchService::Stub> stub_;
};


int main(int argc, char** argv) {
    ExampleClient client(grpc::CreateChannel(
        "localhost:9001", grpc::InsecureChannelCredentials()));
    std::string user("Xiao Liang");
    std::string reply = client.Search(user);    // The actual RPC call
    std::cout << "client received: " << reply << std::endl;

    return 0;
}


