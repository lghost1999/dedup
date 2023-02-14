#include "dedupservice_impl.h"
#include "fringerprint.h"
#include "indexfile.h"
#include "bitmap.h"
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
        std::cout << bitmap.getBit(i) << std::endl;
    }

    
    IndexFile::writeFringerprints(remote_filename, backup_time, fps);

    response->set_result(true);
}
