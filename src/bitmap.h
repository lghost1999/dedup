#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdlib.h>
#include <vector>

#define BITMAP_SHIFT 5
#define BITMAP_MASK 0x1F

class BitMap {
public:
    BitMap(size_t size);

    void set(size_t n);

    bool get(size_t n);

    void clear();

private:
    std::vector<uint32_t> bits_;
};

#endif
