#include "dedupservice_impl.h"
#include "fringerprint.h"
#include "indexfile.h"
#include <iostream>

void DedupServiceImpl::Backup(google::protobuf::RpcController* cntl_base,
                              const dedup::BackupRequest* request,
                              dedup::BackupResponse* response,
                              google::protobuf::Closure* done) {

    brpc::ClosureGuard done_guard(done);

    brpc::Controller* cntl = static_cast<brpc::Controller*>(cntl_base);
    cntl->response_attachment().append(cntl->request_attachment());
    
    std::string remote_filename = request->remote_filename();
    std::string backup_time = request->backup_time();
    std::vector<Fringerprint> fps;

    for (int i = 0; i < request->fringerprint_size(); i++) {
        fps.emplace_back(XXH128_hash_t{request->fringerprint(i).low(), request->fringerprint(i).high()});
    }

    IndexFile::writeFringerprints(remote_filename, backup_time, fps);

    response->set_result(true);
    response->set_error_message("Yes");
    
}
