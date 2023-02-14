#ifndef CHUNK_H
#define CHUNK_H

#include "fringerprint.h"

#include <xxhash.h>
#include <string>

class Chunk {
public:
    Chunk(std::string path, uint32_t offset, uint32_t length, int refcnt, Fringerprint fringerprint);

    void setPath(std::string path);
    std::string getPath() const;

    void setOffset(uint32_t offset);
    uint32_t getOffset() const;

    void setLength(uint32_t length);
    uint32_t getLength() const;

    void setRefcnt(int refcnt);
    int getRefcnt() const;
    void addRefcnt();
    void subRefCnt();

    void setFringerprint(Fringerprint fringerprint);
    Fringerprint getFringerprint() const;

private:
    std::string path_;
    uint32_t offset_;
    uint32_t length_;
    int refcnt_;
    Fringerprint fringerprint_;
};

#endif
