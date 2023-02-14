#include "fastcdc.h"
#include "chunk.h"
#include "fringerprint_lru.h"
#include "bitmap.h"
#include "timeutils.h"

#include <iostream>
#include <vector>
#include <string.h>
#include <functional>

#include <gflags/gflags.h>
#include <butil/logging.h>
#include <butil/time.h>
#include <brpc/channel.h>
#include "dedup_cmd.pb.h"
#include "dedupservice_impl.h"

DEFINE_string(protocol, "baidu_std", "Protocol type. Defined in src/brpc/options.proto");
DEFINE_string(connection_type, "", "Connection type. Available values: single, pooled, short");
DEFINE_string(server, "0.0.0.0:8000", "IP Address of server");
DEFINE_string(load_balancer, "", "The algorithm for load balancing");
DEFINE_int32(timeout_ms, 3000, "RPC timeout in milliseconds");
DEFINE_int32(max_retry, 3, "Max retries(not including the first RPC)");

FastCDC fastcdc(64 * 1024, 256 * 1024, 512 * 1024);
FringerprintLRU lru(1024 * 1024);
brpc::ChannelOptions options;

void backup(std::string local_filename, std::string remote_filename) {
    brpc::Channel channel;
    if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return;
    }

    dedup::DedupService_Stub stub(&channel);
    dedup::QueryFringerprintRequest request;
    dedup::QueryFringerprintResponse response;
    brpc::Controller cntl;

    std::vector<Chunk> chunks;
    fastcdc.parse(local_filename.c_str(), chunks);
    std::cout << "total chunks : " << chunks.size()  << std::endl;

    std::vector<Fringerprint> fps;
    for(auto& chunk : chunks) {
        fps.emplace_back(chunk.getFringerprint());
    }
    BitMap bitmap(fps.size());
    lru.get(fps, bitmap);
    int exist_chunks = 0;
    for (size_t i = 0; i < fps.size(); i++) {
        if (bitmap.get(i)) {
            exist_chunks++;
        }
    }
    lru.put(fps);
    std::cout << "client cache chunks : " << exist_chunks  << std::endl;

    request.set_remote_filename(remote_filename);
    request.set_backup_time(TimesUtil::getDateTime());
    
    for (size_t i = 0; i < bitmap.getLength(); i++) {
        request.add_bit(bitmap.getBit(i));
        std::cout << bitmap.getBit(i) << std::endl;
    }

    dedup::Fringerprint* fringerprint;
    for (const auto& chunk : chunks) {
        fringerprint = request.add_fringerprint();
        fringerprint->set_high(chunk.getFringerprint().getHigh64());
        fringerprint->set_low(chunk.getFringerprint().getLow64());
    }

    stub.QueryFringerprint(&cntl, &request, &response, NULL);
    if (!cntl.Failed()) {
        LOG(INFO) << response.result();
    } else {
        LOG(WARNING) << cntl.ErrorText();
    }
}

void restore(std::string remote_filename, std::string local_filename) {

}




int main(int argc, char* argv[]) {
    
    // test uint8_t to string
    // int offset = 0;
    // uint8_t buf[512 * 1024] = {0};
    // for (auto& chunk : chunks) {
    //     fread(buf, 1, chunks[0].getLength(), file);
    //     std::string t(buf, buf + chunks[0].getLength());
    //     uint8_t* px = (uint8_t*)t.c_str();
        
    //     offset += chunks[0].getLength();
    //     fseek(file, offset, SEEK_SET);
    // }


    // int64_t end_time  = TimesUtil::getTimeNs();
    // double time = (end_time - start_time) / 1e6;
    
    // printf("\n======\n%.2fMB in %.2fMS (%.3f MB/S)\n", 1.0 * fastcdc.getPos() / (1024*1024), time, 1000.0 * fastcdc.getPos() /(time * 1024 * 1024));

    google::ParseCommandLineFlags(&argc, &argv, true);
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms;
    options.max_retry = FLAGS_max_retry;

    std::string cmd, local_filename, remote_filename;
    while(1) {
        std::cin >> cmd >> local_filename >> remote_filename;
        std::cout << cmd << "--" << local_filename << "--" << remote_filename << std::endl;
        if (cmd == "backup") {
            backup(local_filename, remote_filename);
        }
    }
    
    
    return 0;
}

