#ifndef FRINGERPRINT_H
#define FRINGERPRINT_H

#include <xxhash.h>
#include <string>

class Fringerprint {
public:
    Fringerprint() {}

    explicit Fringerprint(XXH128_hash_t fp):fp_(fp) {} 

    bool operator==(const Fringerprint& rhs) const {
        return XXH128_isEqual(fp_, rhs.fp_);
    }

    bool operator!=(const Fringerprint& rhs) const {
        return !operator==(rhs);
    }
    
    // 重载比较运算符，在rocksdb中按key排序
    bool operator<(const Fringerprint& rhs) const {
        if (fp_.high64 == rhs.fp_.high64) {
            return fp_.low64 <= rhs.fp_.high64;
        }
        return fp_.high64 <= rhs.fp_.high64;
    }

    static Fringerprint get(const void* data, size_t len, XXH64_hash_t seed);

    std::string val() const;

private:
    XXH128_hash_t fp_;
};


#endif
