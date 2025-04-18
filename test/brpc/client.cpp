#include <gflags/gflags.h>
#include <butil/logging.h>
#include <butil/time.h>
#include <brpc/channel.h>
#include "echo.pb.h"

DEFINE_string(attachment, "", "Carry this along with requests");
DEFINE_string(protocol, "baidu_std", "Protocol type. Defined in src/brpc/options.proto");
DEFINE_string(connection_type, "", "Connection type. Available values: single, pooled, short");
DEFINE_string(server, "0.0.0.0:8000", "IP Address of server");
DEFINE_string(load_balancer, "", "The algorithm for load balancing");
DEFINE_int32(timeout_ms, 100, "RPC timeout in milliseconds");
DEFINE_int32(max_retry, 3, "Max retries(not including the first RPC)"); 
DEFINE_int32(interval_ms, 1000, "Milliseconds between consecutive requests");

int main(int argc, char* argv[]) {
    // Parse gflags. We recommend you to use gflags as well.
    google::ParseCommandLineFlags(&argc, &argv, true);
    
    // A Channel represents a communication line to a Server. Notice that 
    // Channel is thread-safe and can be shared by all threads in your program.
    brpc::Channel channel;
    
    // Initialize the channel, NULL means using default options.
    brpc::ChannelOptions options;
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms;
    options.max_retry = FLAGS_max_retry;
    if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }

    // Normally, you should not call a Channel directly, but instead construct
    // a stub Service wrapping it. stub can be shared by all threads as well.
    text::EchoService_Stub stub(&channel);

    // 调用回显rpc
    // int log_id = 0;
    // while (!brpc::IsAskedToQuit()) {
    //     // We will receive response synchronously, safe to put variables
    //     // on stack.
    //     text::EchoRequest request;
    //     text::EchoResponse response;
    //     brpc::Controller cntl;

    //     request.set_message("hello world");

    //     cntl.set_log_id(log_id ++);
    //     cntl.request_attachment().append(FLAGS_attachment);

    //     // 同步调用
    //     stub.Echo(&cntl, &request, &response, NULL);
    //     if (!cntl.Failed()) {
    //         LOG(INFO) << "Received response from " << cntl.remote_side()
    //             << " to " << cntl.local_side()
    //             << ": " << response.message() << " (attached="
    //             << cntl.response_attachment() << ")"
    //             << " latency=" << cntl.latency_us() << "us";
    //     } else {
    //         LOG(WARNING) << cntl.ErrorText();
    //     }
    //     usleep(FLAGS_interval_ms * 1000L);
    // }

    // 调用加法rpc
    text::AddRequest request;
    text::AddResponse response;
    brpc::Controller cntl;

    request.set_a(1);
    request.set_b(2);

    stub.add(&cntl, &request, &response, NULL);
    if (!cntl.Failed()) {
        LOG(INFO) << "a + b = " << response.res();
    } else {
        LOG(WARNING) << cntl.ErrorText();
    }

    return 0;
}
