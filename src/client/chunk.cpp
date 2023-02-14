#include "chunk.h"

Chunk::Chunk(std::string path, uint32_t offset, uint32_t length, int refcnt, Fringerprint fringerprint) {
    path_ = path;
    offset_ = offset;
    length_ = length;
    refcnt_ = refcnt;
    fringerprint_ = fringerprint;
}

void Chunk::setPath(std::string path) {
    path_ = path;
}
std::string Chunk::getPath() const {
    return path_;
}

void Chunk::setOffset(uint32_t offset) {
    offset_ = offset;
}
uint32_t Chunk::getOffset() const {
    return offset_;
}

void Chunk::setLength(uint32_t length) {
    length_ = length;
}

uint32_t Chunk::getLength() const {
    return length_;
}

void Chunk::setRefcnt(int refcnt) {
    refcnt_ = refcnt;
}

int Chunk::getRefcnt() const {
    return refcnt_;
}

void Chunk::addRefcnt() {
    refcnt_++;
}

void Chunk::subRefCnt() {
    refcnt_--;
}

void Chunk::setFringerprint(Fringerprint fringerprint) {
    fringerprint_ = fringerprint;
}

Fringerprint Chunk::getFringerprint() const {
    return fringerprint_;
}
