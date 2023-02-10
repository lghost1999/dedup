#include <xxhash.h>
#include <stdlib.h>
#include <stdio.h>
// XXH128_hash_t
// XXH128_isEqual(XXH128_hash_t h1, XXH128_hash_t h2)
// XXH_PUBLIC_API XXH128_hash_t XXH3_128bits(const void* data, size_t len);
// XXH_PUBLIC_API XXH128_hash_t XXH3_128bits_withSeed(const void* data, size_t len, XXH64_hash_t seed);
// XXH_PUBLIC_API XXH128_hash_t XXH3_128bits_withSecret(const void* data, size_t len, const void* secret, size_t secretSize);

int main() {
    const char data1[] = {'a', 'b', 'c', 'd'};
    const char data2[] = {'a', 'b', 'c', 'd', 'e'};
    XXH128_hash_t h1 = XXH3_128bits_withSeed(data1, 4, 1);
    XXH128_hash_t h2 = XXH3_128bits_withSeed(data1, 4, 0);

    printf("h1 : %0lx%0lx\n", h1.high64, h1.low64);
    printf("h2 : %0lx%0lx\n", h2.high64, h2.low64);
    
    if (XXH128_isEqual(h1, h2)) {
        printf("Equal\n");
    } else {
        printf("No\n");
    }

    return 0;
}
