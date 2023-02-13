#ifndef DBCHUNK_H
#define DBCHUNK_H

#include "fringerprint.h"

class DBChunk {
public:
    DBChunk(int file_id = 0, int offset = 0, int length = 0, int refcnt = 0);

    int getFileid() const;
    void setFileid(int file_id);

    uint32_t getOffset() const;
    void setOffset(uint32_t offset);

    uint32_t getLength() const;
    void setLength(uint32_t length);

    int getRefcnt() const;
    void setRefcnt(uint32_t refcnt);

    void addRefcnt();
    void subRefcnt();
    
private:
    int file_id_;
    uint32_t offset_;
    uint32_t length_;
    int refcnt_;
};

#endif
