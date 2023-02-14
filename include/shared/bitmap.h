#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdlib.h>
#include <vector>

#define BITMAP_SHIFT 6
#define BITMAP_MASK 0x3F

class BitMap {
public:
    BitMap(size_t size = 0);

    void set(size_t n);

    bool get(size_t n);

    void clear();
    
    void setLength(size_t length);
    size_t getLength();

    void setBit(size_t idx, size_t val);
    size_t getBit(size_t idx);

private:
    std::vector<size_t> bits_;
};

#endif
