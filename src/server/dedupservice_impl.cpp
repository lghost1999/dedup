#include "dedupservice_impl.h"
#include "fringerprint.h"
#include "indexfile.h"
#include "bitmap.h"
#include "bloomfilter.h"
#include "file_writer.h"
#include "dbhelper.h"
#include <iostream>

void DedupServiceImpl::QueryFringerprint(google::protobuf::RpcController* cntl_base,
                                         const dedup::QueryFringerprintRequest* request,
                                         dedup::QueryFringerprintResponse* response,
                                         google::protobuf::Closure* done) {

    brpc::ClosureGuard done_guard(done);

    brpc::Controller* cntl = static_cast<brpc::Controller*>(cntl_base);
    cntl->response_attachment().append(cntl->request_attachment());
    
    std::string remote_filename = request->remote_filename();
    std::string backup_time = request->backup_time();
    std::vector<Fringerprint> fps;
    BitMap bitmap;

    for (int i = 0; i < request->fringerprint_size(); i++) {
        fps.emplace_back(XXH128_hash_t{request->fringerprint(i).low(), request->fringerprint(i).high()});
    }

    
    bitmap.setLength(request->bit_size());
    for (int i = 0; i < request->bit_size(); i++) {
        bitmap.setBit(i, request->bit(i));
    }

    // 批量查询BloomFilter
    extern BloomFilter bloomfilter;
    if (!bloomfilter.get(fps, bitmap)) {
        response->set_result(false);
        cntl->request_attachment().append("Query BloomFilter failed");
        return;
    }

    // 批量更新BloomFilter
    if (!bloomfilter.set(fps, bitmap)) {
        response->set_result(false);
        cntl->request_attachment().append("Update BloomFilter failed");
        return;
    }

    if (!IndexFile::writeFringerprints(remote_filename, backup_time, fps)) {
        response->set_result(false);
        cntl->request_attachment().append("Write index file failed");
    }


    response->set_result(true);
    for (int i = 0; i < request->bit_size(); i++) {
        response->add_bit(bitmap.getBit(i));
    }
    
}

void DedupServiceImpl::StorageChunk(google::protobuf::RpcController* cntl_base,
                                    const dedup::StorageChunkRequest* request,
                                    dedup::StorageChunkResponse* response,
                                    google::protobuf::Closure* done) {
    
    brpc::ClosureGuard done_guard(done);

    brpc::Controller* cntl = static_cast<brpc::Controller*>(cntl_base);
    cntl->response_attachment().append(cntl->request_attachment());

    FileWriter* filewriter = FileWriter::getInstance(64 * 1024 * 1024);
    DBHelper helper("./DB");
    helper.openDB();
    for (int i = 0; i < request->chunk_size(); i++) {
        DBChunk chunk;
        if (!filewriter->write(std::move(request->chunk(i)), chunk)) {
            response->set_result(false);
            cntl->request_attachment().append("Write Data File failed");
            return;
        }
        Fringerprint fp(XXH128_hash_t{request->fringerprint(i).low(), request->fringerprint(i).high()});
        if (!helper.put(fp, chunk)) {
            response->set_result(false);
            cntl->request_attachment().append("Write RocksDB failed");
            return;
        }
    }
    helper.closeDB();
    response->set_result(true);
}

void DedupServiceImpl::RestoreFile(google::protobuf::RpcController* cntl_base,
                                    const dedup::RestoreFileRequest* request,
                                    dedup::RestoreFileResponse* response,
                                    google::protobuf::Closure* done) {
    brpc::ClosureGuard done_guard(done);

    brpc::Controller* cntl = static_cast<brpc::Controller*>(cntl_base);
    cntl->response_attachment().append(cntl->request_attachment());

    std::string remote_filename = request->remote_filename();
    std::string backup_time = request->backup_time();
    std::string file_name = remote_filename;

    FILE *file = fopen(file_name.c_str(), "r");
    if (!file) {
        response->set_result(false);
        cntl->request_attachment().append("Fail to Open Index File");
        return;
    }
    
    DBHelper helper("./DB");
    helper.openDB();
    std::vector<DBChunk> chunks;
    std::string file_data;
    
    while (!feof(file)) {
        uint64_t high, low;
        if (fscanf(file,"%lu-%lu\n", &high, &low) == 1) {
            response->set_result(false);
            cntl->request_attachment().append("Read Index File error");
            return;
        }

        Fringerprint fp({low, high});
        DBChunk chunk;
        if (!helper.get(fp, chunk)) {
            response->set_result(false);
            cntl->request_attachment().append("Fail to get Chunk Info");
            return;
        }
        
        chunks.emplace_back(chunk);
    }
    helper.closeDB();
    fclose(file);

    if (!IndexFile::readChunks(file_data, chunks)) {
        response->set_result(false);
        cntl->request_attachment().append("Fail to read Chunk");
        return;
    }

    response->set_result(true);
    response->set_file_data(file_data);

}
