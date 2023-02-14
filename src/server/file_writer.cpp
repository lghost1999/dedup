#include "file_writer.h"
#include <fstream>
#include <butil/logging.h>
#include <iostream>

FileWriter* FileWriter::getInstance(uint32_t capacity) {
    static FileWriter filewriter(capacity);
    return &filewriter;
}

bool FileWriter::write(std::string chunkdata, DBChunk& dbchunk) {
    std::string filename = "datafile" + std::to_string(file_id_);
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::app);
    if (!file) {
        LOG(ERROR) << "fail to open" << filename;
        return false;
    }

    if (size_ + chunkdata.size() > capacity_) {
        file.close();
        ++file_id_;
        size_ = 0;

        filename = "datafile" + std::to_string(file_id_);
        file.open(filename, std::ios::out | std::ios::app);
        if (!file) {
            LOG(ERROR) << "fail to open" << filename;
            return false;
        }
    }

    file << chunkdata;
    file.close();
    
    dbchunk.setFileid(file_id_);
    dbchunk.setOffset(size_);
    dbchunk.setLength(chunkdata.size());
    dbchunk.setRefcnt(1);

    size_ += chunkdata.size();
    
    return true;
}
