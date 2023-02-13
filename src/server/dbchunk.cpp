#include "dbchunk.h"

DBChunk::DBChunk(int file_id, int offset, int length, int refcnt) {
    file_id_ = file_id;
    offset_ = offset;
    length_ = length;
    refcnt_ = refcnt;
}

int DBChunk::getFileid() const {
    return file_id_;
}

void DBChunk::setFileid(int file_id) {
    file_id_ = file_id;
}

uint32_t DBChunk::getOffset() const {
    return offset_;
}

void DBChunk::setOffset(uint32_t offset) {
    offset_ = offset;
}

uint32_t DBChunk::getLength() const {
    return length_;
}

void DBChunk::setLength(uint32_t length) {
    length_ = length;
}

int DBChunk::getRefcnt() const {
    return refcnt_;
}

void DBChunk::setRefcnt(uint32_t refcnt) {
    refcnt_ = refcnt;
}

void DBChunk::addRefcnt() {
    ++refcnt_;
}

void DBChunk::subRefcnt() {
    --refcnt_;
}
