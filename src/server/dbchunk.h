#ifndef DBCHUNK_H
#define DBCHUNK_H

#include "fringerprint.h"

class DBChunk {
public:
    DBChunk(int file_id, int offset, int length, int refcnt);

    int getFileid() const;
    void setFileid(int file_id);

    int getOffset() const;
    void setOffset(int offset);

    int getLength() const;
    void setLength(int length_);

    int getRefcnt() const;
    void setRefcnt(int refcnt);
    void addRefcnt();
    void subRefcnt();
    
private:
    int file_id_;
    int offset_;
    int length_;
    int refcnt_;
};

#endif
