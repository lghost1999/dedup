#include "fastcdc.h"
#include "chunk.h"
#include "fringerprint_lru.h"
#include "bitmap.h"
#include <iostream>
#include <vector>
#include <time.h>

int main(int argc, char* argv[]) {
    FastCDC fastcdc(64 * 1024, 256 * 1024, 512 * 1024);
    std::vector<Chunk> chunks;

    struct timespec tp1, tp2;

    clock_gettime(CLOCK_MONOTONIC, &tp1);
    fastcdc.parse(argv[1], chunks);
    
    // for (auto& chunk : chunks) {
    //     std::cout << chunk.getPath() << " : " << chunk.getOffset() << " : "
    //               << chunk.getLength() <<" : " << chunk.getFringerprint().val() << std::endl;
    // }

    std::vector<Fringerprint> fps;
    for (auto& chunk : chunks) {
        fps.emplace_back(chunk.getFringerprint());
    }

    FringerprintLRU lru(3000);
    
    BitMap bitmap(2000);

    std::cout << " ---- " << std::endl;
    lru.put(fps);
    std::cout << lru.getsize() << std::endl;
    lru.get(fps, bitmap);

    // for (size_t i = 0; i < 1000; i++) {
    //     if (bitmap.get(i)) {
    //         std::cout << i << std::endl;
    //     }
    // }



    clock_gettime(CLOCK_MONOTONIC, &tp2);
    double elapsed = (tp2.tv_sec - tp1.tv_sec)*1000 + (tp2.tv_nsec - tp1.tv_nsec)/1000000.0;
    printf("\n======\n%.2fMB in %.2fMS (%.3f MB/S)\n", 1.0 * fastcdc.getPos()/(1024*1024), elapsed, 1000.0*fastcdc.getPos()/(elapsed*1024*1024));


    return 0;
}

