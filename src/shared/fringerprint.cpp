#include "fringerprint.h"

#include <iostream>
#include <sstream>
#include <iomanip>

Fringerprint Fringerprint::none;

Fringerprint Fringerprint::get(const void* data, size_t len, XXH64_hash_t seed) {
    return Fringerprint(XXH3_128bits_withSeed(data, len, seed));
}

std::string Fringerprint::val() const {
    std::ostringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(16) << fp_.high64
       << std::setfill('0') << std::setw(16) << fp_.low64;
    return ss.str();
}


