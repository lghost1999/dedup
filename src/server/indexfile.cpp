#include "indexfile.h"

#include <butil/logging.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#define MODE (S_IRWXU | S_IRWXG | S_IRWXO)  

bool IndexFile::writeFringerprints(std::string& dir_name, std::string& file_name, std::vector<Fringerprint>& fps) {
    if (access(dir_name.c_str(), F_OK) == -1) {
        if (mkdir(dir_name.c_str(), MODE) == -1) {
            LOG(ERROR) << "fail to create folder " << dir_name.c_str();
            return false;
        }
    }   

    std::string file_path = dir_name + "/" + file_name;
    FILE* file = fopen(file_path.c_str(), "w");
    if (file == NULL) {
        LOG(ERROR) << "fail to create file " << file_name.c_str();
        return false;
    }

    for (auto& fp : fps) {
        fprintf(file, "%zu-%zu\n", fp.getHigh64(), fp.getLow64());
    }

    fclose(file);
    return true;
}

bool IndexFile::readChunks(std::string& data, std::vector<DBChunk>& dbchunks) {
    char buf[512 * 1024] = {0};
    for (auto dbchunk : dbchunks) {
        std::string file_name = "datafile" + std::to_string(dbchunk.getFileid());
        FILE *file = fopen(file_name.c_str(), "r");
        if (!file) {
            LOG(ERROR) << "fail to open" << file_name;
            return false;
        }

        fseek(file, dbchunk.getOffset(), SEEK_SET);
        if (!fread(buf, 1, dbchunk.getLength(), file)) {
            LOG(ERROR) << "fail to read" << file_name;
            return false;
        }
        data += std::string(buf, buf + dbchunk.getLength());
        fclose(file);
    }
    return true;
}
