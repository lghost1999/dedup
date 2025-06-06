#ifndef FRINGERPRINT_H
#define FRINGERPRINT_H

#include <xxhash.h>
#include <string>

class Fringerprint {
public:
    explicit Fringerprint(XXH128_hash_t fp = {0, 0}):fp_(fp) {} 

    bool operator==(const Fringerprint& rhs) const {
        return XXH128_isEqual(fp_, rhs.fp_);
    }

    bool operator!=(const Fringerprint& rhs) const {
        return !operator==(rhs);
    }
    
    // 自定义哈希函数
    size_t operator() (const Fringerprint& rhs) const {
        return rhs.fp_.high64 ^ rhs.fp_.low64;
    }

    // 重载比较运算符，在rocksdb中按key排序
    bool operator<(const Fringerprint& rhs) const {
        return fp_.high64 < rhs.fp_.high64 || (fp_.high64 == rhs.fp_.high64 && fp_.low64 < rhs.fp_.low64);
    }

    size_t getHigh64() {
        return fp_.high64;
    }

    size_t getLow64() {
        return fp_.low64;
    }
    
    std::string val() const;

    static Fringerprint get(const void* data, size_t len, XXH64_hash_t seed);

    static Fringerprint none;

private:
    XXH128_hash_t fp_;
};


#endif
