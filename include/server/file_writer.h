#ifndef DATAFILE_H
#define DATAFILE_H

#include "chunk.h"
#include "dbchunk.h"
#include <stdint.h>

// 优化：可以先写入缓冲区，缓冲区满再写入文件
class FileWriter {
public:
    ~FileWriter(){};
    
    FileWriter(const FileWriter& fileWriter) = delete;
    FileWriter& operator=(const FileWriter& fileWriter) = delete;

    static FileWriter* getInstance(uint32_t capacity);

    bool write(std::string& chunk, DBChunk& dbchunk);

private:
    FileWriter(uint32_t capacity_): file_id_(0), file_(nullptr), size_(0), capacity_(capacity_){};

    int file_id_;
    FILE* file_;
    uint32_t size_;
    uint32_t capacity_;
};
#endif
