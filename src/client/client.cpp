#include "fastcdc.h"
#include "chunk.h"
#include "fringerprint_lru.h"
#include "bitmap.h"
#include "timeutils.h"

#include <iostream>
#include <vector>
#include <string.h>
#include <functional>
#include <fstream>

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

void backup(std::string& local_filename, std::string& remote_filename) {
    brpc::Channel channel;
    if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return;
    }

    dedup::DedupService_Stub stub(&channel);
    dedup::QueryFringerprintRequest query_request;
    dedup::QueryFringerprintResponse query_response;
    brpc::Controller cntl;

    std::vector<Chunk> chunks;
    int64_t start_time  = TimesUtil::getTimeNs();
    fastcdc.parse(local_filename.c_str(), chunks);
    double elapse  = (TimesUtil::getTimeNs() - start_time) / 1e6;
    printf("%.2fMB in %.2fMS (%.3f MB/S)\n", 1.0 * fastcdc.getPos() / (1024 * 1024), elapse, 1000.0 * fastcdc.getPos() /(elapse * 1024 * 1024));
    LOG(INFO) << "total chunks : " << chunks.size()  << std::endl;

    std::vector<Fringerprint> fps;
    for(auto& chunk : chunks) {
        fps.emplace_back(chunk.getFringerprint());
    }

    BitMap bitmap(fps.size());
    lru.get(fps, bitmap);
    int client_chunks = 0;
    for (size_t i = 0; i < fps.size(); i++) {
        if (bitmap.get(i)) {
            client_chunks++;
        }
    }
    lru.put(fps);
    LOG(INFO) << "client cache chunks : " << client_chunks;

    query_request.set_remote_filename(remote_filename);
    query_request.set_backup_time(TimesUtil::getDateTime());
    for (size_t i = 0; i < bitmap.getLength(); i++) {
        query_request.add_bit(bitmap.getBit(i));
    }

    dedup::Fringerprint* fringerprint;
    for (const auto& chunk : chunks) {
        fringerprint = query_request.add_fringerprint();
        fringerprint->set_high(chunk.getFringerprint().getHigh64());
        fringerprint->set_low(chunk.getFringerprint().getLow64());
    }

    stub.QueryFringerprint(&cntl, &query_request, &query_response, NULL);
    if (!cntl.Failed()) {
        LOG(INFO) << "QueryFringerprint RPC OK";
    } else {
        LOG(ERROR) << cntl.request_attachment();
        return;
    }

    for (int i = 0; i < query_response.bit_size(); i++) {
        bitmap.setBit(i, query_response.bit(i));
    }

    int server_chunks = 0;
    for (size_t i = 0; i < fps.size(); i++) {
        if (bitmap.get(i)) {
            server_chunks++;
        }
    }
    LOG(INFO) << "server cache chunks : " << server_chunks - client_chunks  <<
                 "storage chunks : " << chunks.size() - server_chunks;

    dedup::StorageChunkRequest storage_request;
    dedup::StorageChunkResponse storage_response;

    FILE *file = fopen(local_filename.c_str(), "r");
    if (!file) {
        LOG(ERROR) << "fail to open" << local_filename;
        return;
    }
    

    char buf[512 * 1024] = {0};
    int offset = 0;
    for (size_t i = 0; i < chunks.size(); i++) {
        if (!bitmap.get(i)) {
            if (!fread(buf, 1, chunks[i].getLength(), file)) {
                LOG(ERROR) << "fail to read" << local_filename << "--" << offset;
                return;
            }
            std::string data(buf, buf + chunks[i].getLength());
            storage_request.add_chunk(data);

            fringerprint = storage_request.add_fringerprint();
            fringerprint->set_high(chunks[i].getFringerprint().getHigh64());
            fringerprint->set_low(chunks[i].getFringerprint().getLow64());
        }
        offset += chunks[i].getLength();
        fseek(file, offset, SEEK_SET);
    }
    fclose(file);

    cntl.Reset();
    stub.StorageChunk(&cntl, &storage_request, &storage_response, NULL);
    if (!cntl.Failed()) {
        LOG(INFO) << "Storage Chunk RPC OK";
    } else {
        LOG(ERROR) << cntl.request_attachment();
        return;
    }

}

void restore(std::string remote_filename, std::string local_filename) {
    brpc::Channel channel;
    if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return;
    }

    dedup::DedupService_Stub stub(&channel);
    dedup::RestoreFileRequest restore_request;
    dedup::RestoreFileResponse restore_response;
    brpc::Controller cntl;

    restore_request.set_remote_filename(remote_filename);

    stub.RestoreFile(&cntl, &restore_request, &restore_response, NULL);
    if (!cntl.Failed()) {
        LOG(INFO) << "Restore FILE RPC OK";
    } else {
        LOG(ERROR) << cntl.request_attachment();
        return;
    }

    std::string data = restore_response.file_data();
    std::ofstream file;
    file.open(local_filename, std::ios::out | std::ios::app);
    if (!file) {
        LOG(ERROR) << "fail to open" << local_filename;
        return;
    }
    file << data;
    file.close();
}

int main(int argc, char* argv[]) {

    google::ParseCommandLineFlags(&argc, &argv, true);
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms;
    options.max_retry = FLAGS_max_retry;

    std::string cmd, local_filename, remote_filename;
    while(1) {
        std::cin >> cmd >> local_filename >> remote_filename;
        if (cmd == "backup") {
            backup(local_filename, remote_filename);
        } else if (cmd == "restore") {
            restore(remote_filename, local_filename);
        }
    }
    
    return 0;
}

