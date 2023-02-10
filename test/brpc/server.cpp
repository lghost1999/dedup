#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include "echo.pb.h"

// 设置参数 DEFINE_*
DEFINE_bool(echo_attachment, true, "Echo attachment as well");
DEFINE_int32(port, 8000, "TCP Port of this server");
DEFINE_string(listen_addr, "", "Server listen address, may be IPV4/IPV6/UDS."
            " If this is set, the flag port will be ignored");
DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no "
             "read/write operations during the last `idle_timeout_s'");


class EchoServiceImpl : public text::EchoService {
public:
    EchoServiceImpl() {}
    virtual ~EchoServiceImpl() {}
    virtual void Echo(google::protobuf::RpcController* cntl_base,
                      const text::EchoRequest* request,
                      text::EchoResponse* response,
                      google::protobuf::Closure* done) {
        // 使用RAII机制, 在析构时自动调用done->run()
        brpc::ClosureGuard done_guard(done);

        //rpc控制器信息，记录错误信息
        brpc::Controller* cntl =
            static_cast<brpc::Controller*>(cntl_base);

        // 设置响应
        response->set_message(request->message() + "hahaha");

        // You can compress the response by setting Controller, but be aware
        // that compression may be costly, evaluate before turning on.
        // cntl->set_response_compress_type(brpc::COMPRESS_TYPE_GZIP);

        // if (FLAGS_echo_attachment) {
        //     // Set attachment which is wired to network directly instead of
        //     // being serialized into protobuf messages.
        //     cntl->response_attachment().append(cntl->request_attachment());
        // }
    }

    virtual void add(google::protobuf::RpcController* cntl_base,
                      const text::AddRequest* request,
                      text::AddResponse* response,
                      google::protobuf::Closure* done) {
        brpc::ClosureGuard done_guard(done);

        //rpc控制器信息，记录错误信息
        brpc::Controller* cntl =
            static_cast<brpc::Controller*>(cntl_base);

        int a = request->a();
        int b = request->b();

        // 设置响应
        response->set_res(a + b);

        // You can compress the response by setting Controller, but be aware
        // that compression may be costly, evaluate before turning on.
        // cntl->set_response_compress_type(brpc::COMPRESS_TYPE_GZIP);

        // if (FLAGS_echo_attachment) {
        //     // Set attachment which is wired to network directly instead of
        //     // being serialized into protobuf messages.
        //     cntl->response_attachment().append(cntl->request_attachment());
        // }
    }
};


int main(int argc, char* argv[]) {
    // Parse gflags. We recommend you to use gflags as well.
    google::ParseCommandLineFlags(&argc, &argv, true);

    brpc::Server server;

    EchoServiceImpl echo_service_impl;

    // SERVER_OWNS_SERVICE会将add失败的Service进行delete删除
    if (server.AddService(&echo_service_impl, 
                          brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        LOG(ERROR) << "Fail to add service";
        return -1;
    }

    butil::EndPoint point;
    if (!FLAGS_listen_addr.empty()) {
        if (butil::str2endpoint(FLAGS_listen_addr.c_str(), &point) < 0) {
            LOG(ERROR) << "Invalid listen address:" << FLAGS_listen_addr;
            return -1;
        }
    } else {
        point = butil::EndPoint(butil::IP_ANY, FLAGS_port);
    }
    // Start the server.
    brpc::ServerOptions options;
    options.idle_timeout_sec = FLAGS_idle_timeout_s;
    if (server.Start(point, &options) != 0) {
        LOG(ERROR) << "Fail to start EchoServer";
        return -1;
    }

    // Wait until Ctrl-C is pressed, then Stop() and Join() the server.
    server.RunUntilAskedToQuit();
    return 0;
}
