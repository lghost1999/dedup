#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include "bitmap.h"
#include "fringerprint.h"

#define BLOOMFILTER_SHIFT 32
#define BLOOMFILTER_MASK 0xFFFFFFFF

class BloomFilter {
public:
    BloomFilter(size_t size) : bitmap_(size) {};

    void set(Fringerprint fp);

    bool get(Fringerprint fp);

    size_t hash1(Fringerprint fp);
    size_t hash2(Fringerprint fp);
    size_t hash3(Fringerprint fp);
    
private:
    BitMap bitmap_;
};
#endif
