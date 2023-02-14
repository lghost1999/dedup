#include "bloomfilter.h"
#include "dbhelper.h"
#include "dbchunk.h"
#include "fringerprint.h"
#include "file_writer.h"
#include "dedupservice_impl.h"

#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include <iostream>

DEFINE_bool(echo_attachment, true, "Echo attachment as well");
DEFINE_int32(port, 8000, "TCP Port of this server");
DEFINE_string(listen_addr, "", "Server listen address, may be IPV4/IPV6/UDS. If this is set, the flag port will be ignored");
DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no read/write operations during the last `idle_timeout_s'");

BloomFilter bloomfilter(0xFFFFFFFF);

int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    brpc::Server server;

    DedupServiceImpl dedup_service_impl;

    if (server.AddService(&dedup_service_impl, 
                          brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        LOG(ERROR) << "Fail to add service";
        return -1;
    }

    butil::EndPoint point;
    if (!FLAGS_listen_addr.empty()) {
        if (butil::str2endpoint(FLAGS_listen_addr.c_str(), &point) < 0) {
            LOG(ERROR) << "Invalid listen address:" << FLAGS_listen_addr;
            return -1;
        }
    } else {
        point = butil::EndPoint(butil::IP_ANY, FLAGS_port);
    }

    brpc::ServerOptions options;
    options.idle_timeout_sec = FLAGS_idle_timeout_s;
    if (server.Start(point, &options) != 0) {
        LOG(ERROR) << "Fail to start EchoServer";
        return -1;
    }

    server.RunUntilAskedToQuit();
    
    return 0;
}
