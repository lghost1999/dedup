#include "fastcdc.h"
#include "chunk.h"

#include <iostream>
#include <vector>
#include <time.h>

int main(int argc, char* argv[]) {
    FastCDC fastcdc(64 * 1024, 256 * 1024, 512 * 1024);
    std::vector<Chunk> chunks;

    struct timespec tp1, tp2;

    clock_gettime(CLOCK_MONOTONIC, &tp1);
    fastcdc.parse(argv[1], chunks);
    clock_gettime(CLOCK_MONOTONIC, &tp2);
    double elapsed = (tp2.tv_sec - tp1.tv_sec)*1000 + (tp2.tv_nsec - tp1.tv_nsec)/1000000.0;
    printf("\n======\n%.2fMB in %.2fMS (%.3f MB/S)\n", 1.0 * fastcdc.getPos()/(1024*1024), elapsed, 1000.0*fastcdc.getPos()/(elapsed*1024*1024));

    // for (auto& chunk : chunks) {
    //     std::cout << chunk.getPath() << " : " << chunk.getOffset() << " : " << chunk.getLength() <<
    //                  chunk.getRefcnt() << " : " << chunk.getFringerprint().val() << std::endl;
    // }
    return 0;
}
