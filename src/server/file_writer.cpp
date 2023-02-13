#include "file_writer.h"

#include <butil/logging.h>

FileWriter* FileWriter::getInstance(uint32_t capacity) {
    static FileWriter filewriter(capacity);
    return &filewriter;
}

bool FileWriter::write(std::string& chunkdata, DBChunk& dbchunk) {
    std::string filename = "datafile" + std::to_string(file_id_);
    if (!file_) {
        file_ = fopen(filename.c_str(), "a+");
        if (!file_) {
            LOG(ERROR) << "fail to open" << filename;
            return false;
        }
    }

    if (size_ + chunkdata.size() > capacity_) {
        fclose(file_);
        ++file_id_;
        size_ = 0;

        filename = "datafile" + std::to_string(file_id_);
        file_ = fopen(filename.c_str(), "a+");
        if (!file_) {
            LOG(ERROR) << "fail to open" << filename;
            return false;
        }
    }

    if(!fputs(chunkdata.c_str(), file_)) {
        LOG(ERROR) << "fail to write chunk data" << filename;
        return false;
    }

    dbchunk.setFileid(file_id_);
    dbchunk.setOffset(size_);
    dbchunk.setLength(chunkdata.size());
    dbchunk.setRefcnt(1);

    size_ += chunkdata.size();
    
    return true;
}
