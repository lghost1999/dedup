#include "bitmap.h"

#include <string.h>
#include <iostream>
BitMap::BitMap(size_t size) {
    bits_.resize((size >> BITMAP_SHIFT) + 1, 0);
}


void BitMap::set(size_t n) {
    bits_[n >> BITMAP_SHIFT] |= (1 << (n & BITMAP_MASK));
}

bool BitMap::get(size_t n) {
    return bits_[n >> BITMAP_SHIFT] & (1 << (n & BITMAP_MASK));
}

void BitMap::clear() {
    bits_.clear();
}

