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

int DBChunk::getOffset() const {
    return offset_;
}

void DBChunk::setOffset(int offset) {
    offset_ = offset;
}

int DBChunk::getLength() const {
    return length_;
}

void DBChunk::setLength(int length) {
    length_ = length;
}

int DBChunk::getRefcnt() const {
    return refcnt_;
}

void DBChunk::setRefcnt(int refcnt) {
    refcnt_ = refcnt;
}

void DBChunk::addRefcnt() {
    ++refcnt_;
}

void DBChunk::subRefcnt() {
    --refcnt_;
}
