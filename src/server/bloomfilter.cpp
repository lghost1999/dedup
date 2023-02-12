#include "bloomfilter.h"

void BloomFilter::set(Fringerprint fp) {
    bitmap_.set(hash1(fp));
    bitmap_.set(hash2(fp));
    bitmap_.set(hash3(fp));
}

bool BloomFilter::get(Fringerprint fp) {
    return bitmap_.get(hash1(fp)) && bitmap_.get(hash2(fp)) && bitmap_.get(hash3(fp));
}

size_t BloomFilter::hash1(Fringerprint fp) {
    return fp.getHigh64() & BLOOMFILTER_MASK;
}

size_t BloomFilter::hash2(Fringerprint fp) {
    return fp.getLow64() & BLOOMFILTER_MASK;
}

size_t BloomFilter::hash3(Fringerprint fp) {
    return (fp.getHigh64() >> BLOOMFILTER_SHIFT | fp.getLow64() << BLOOMFILTER_SHIFT) & BLOOMFILTER_MASK;
}
