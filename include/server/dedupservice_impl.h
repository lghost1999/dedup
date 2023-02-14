#include "dedup_cmd.pb.h"

#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>

class DedupServiceImpl : public dedup::DedupService {
public:
    DedupServiceImpl() {}
    virtual ~DedupServiceImpl() {}
    virtual void QueryFringerprint(google::protobuf::RpcController* cntl_base,
                        const dedup::QueryFringerprintRequest* request,
                        dedup::QueryFringerprintResponse* response,
                        google::protobuf::Closure* done);

};
