#include "bitmap.h"

#include <string.h>

BitMap::BitMap(size_t num) {
    bits_ = new uint32_t[(num >> SHIFT) + 1];
    clear();
    if (bits_ == nullptr) {
        exit(-1);
    }
}

BitMap::~BitMap() {
    delete bits_;
}

void BitMap::put(size_t n) {
    bits_[n >> SHIFT] |= (1 << (n & MASK));
}

bool BitMap::get(size_t n) {
    return bits_[n >> SHIFT] & (1 << (n & MASK));
}

void BitMap::clear() {
    if (bits_ != nullptr) {
        memset(bits_, 0, sizeof(*bits_));
    }
}

