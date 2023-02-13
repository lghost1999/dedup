#include "dedupservice_impl.h"

void DedupServiceImpl::Backup(google::protobuf::RpcController* cntl_base,
                              const dedup::BackupRequest* request,
                              dedup::BackupResponse* response,
                              google::protobuf::Closure* done) {

    brpc::ClosureGuard done_guard(done);

    brpc::Controller* cntl =
        static_cast<brpc::Controller*>(cntl_base);

    std::string remote_filename = request->remote_filename();
    std::string backup_time = request->backup_time();

    std::cout << remote_filename << " -- " << backup_time << std::endl;
    
    response->set_result(true);
    response->set_error_message("Yes");
    
}
