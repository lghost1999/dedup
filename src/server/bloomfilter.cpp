#include "bloomfilter.h"
#include "dbhelper.h"

bool BloomFilter::set(std::vector<Fringerprint>& fps, BitMap& bitmap) {
    for (size_t i = 0; i < fps.size(); i++) {
        if (bitmap.get(i)) continue;
        bitmap_.set(hash1(fps[i]));
        bitmap_.set(hash2(fps[i]));
        bitmap_.set(hash3(fps[i]));
    }

    return true;
}

bool BloomFilter::get(std::vector<Fringerprint>& fps, BitMap& bitmap) {
    DBHelper helper("./DB");
    helper.openDB();
    DBChunk chunk;
    for (size_t i = 0; i < fps.size(); i++) {
        if (bitmap.get(i)) continue;
        if (bitmap_.get(hash1(fps[i])) && 
            bitmap_.get(hash2(fps[i])) && 
            bitmap_.get(hash3(fps[i]))) {
            if (helper.get(fps[i], chunk)) {
                bitmap.set(i);
            }
        }   
    }
    
    return true;
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
