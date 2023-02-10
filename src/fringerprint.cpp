#include "fringerprint.h"
#include <iostream>
#include <sstream>

Fringerprint Fringerprint::get(const void* data, size_t len, XXH64_hash_t seed) {
    return Fringerprint(XXH3_128bits_withSeed(data, len, seed));
}

std::string Fringerprint::val() const {
    std::ostringstream ss;
    ss << std::hex << fp_.high64 << fp_.low64;
    return ss.str();
}


