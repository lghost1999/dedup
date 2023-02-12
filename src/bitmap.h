#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdlib.h>

#define SHIFT 5
#define MASK 0x1F

class BitMap {
public:
    BitMap(size_t num);
    ~BitMap();

    void put(size_t n);

    bool get(size_t n);

    void clear();
    
private:
    uint32_t* bits_;
};

#endif
