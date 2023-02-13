#include "dedup_cmd.pb.h"

#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>

class DedupServiceImpl : public dedup::DedupService {
public:
    DedupServiceImpl() {}
    virtual ~DedupServiceImpl() {}
    virtual void Backup(google::protobuf::RpcController* cntl_base,
                        const dedup::BackupRequest* request,
                        dedup::BackupResponse* response,
                        google::protobuf::Closure* done);

};
