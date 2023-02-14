#include "fastcdc.h"
#include "chunk.h"
#include "fringerprint_lru.h"
#include "bitmap.h"
#include "timeutils.h"
#include <iostream>
#include <vector>
#include <string.h>

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

int main(int argc, char* argv[]) {
    
    
    // FILE* file = fopen(argv[1], "r");

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

    // test bitmap
    // std::vector<Fringerprint> fps;
    // for (auto& chunk : chunks) {
    //     fps.emplace_back(chunk.getFringerprint());
    // }

    // FringerprintLRU lru(3000);
    
    // BitMap bitmap(2000);

    // std::cout << " ---- " << std::endl;
    // lru.put(fps);
    // std::cout << lru.getsize() << std::endl;
    // lru.get(fps, bitmap);


    // int64_t end_time  = TimesUtil::getTimeNs();
    // double time = (end_time - start_time) / 1e6;
    
    // printf("\n======\n%.2fMB in %.2fMS (%.3f MB/S)\n", 1.0 * fastcdc.getPos() / (1024*1024), time, 1000.0 * fastcdc.getPos() /(time * 1024 * 1024));

    google::ParseCommandLineFlags(&argc, &argv, true);
    
    brpc::ChannelOptions options;
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms;
    options.max_retry = FLAGS_max_retry;


    FastCDC fastcdc(64 * 1024, 256 * 1024, 512 * 1024);
    std::vector<Chunk> chunks;
    fastcdc.parse(argv[1], chunks);


    brpc::Channel channel;
    if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }

    dedup::DedupService_Stub stub(&channel);
    dedup::BackupRequest request;
    dedup::BackupResponse response;
    brpc::Controller cntl;


    request.set_remote_filename(argv[2]);
    request.set_backup_time(TimesUtil::getDateTime());
    dedup::Fringerprint* fringerprint;

    for (const auto& chunk : chunks) {
        fringerprint = request.add_fringerprint();
        fringerprint->set_high(chunk.getFringerprint().getHigh64());
        fringerprint->set_low(chunk.getFringerprint().getLow64());
    }

    stub.Backup(&cntl, &request, &response, NULL);
    if (!cntl.Failed()) {
        LOG(INFO) << response.error_message();
    } else {
        LOG(WARNING) << cntl.ErrorText();
    }

    return 0;
}

