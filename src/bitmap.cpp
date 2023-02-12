#include "bitmap.h"

#include <string.h>
#include <iostream>
BitMap::BitMap(size_t size) {
    bits_.resize((size >> SHIFT) + 1, 0);
}


void BitMap::put(size_t n) {
    bits_[n >> SHIFT] |= (1 << (n & MASK));
}

bool BitMap::get(size_t n) {
    return bits_[n >> SHIFT] & (1 << (n & MASK));
}

void BitMap::clear() {
    bits_.clear();
}

