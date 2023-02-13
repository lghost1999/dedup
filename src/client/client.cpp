#include "fastcdc.h"
#include "chunk.h"
#include "fringerprint_lru.h"
#include "bitmap.h"
#include "timeutils.h"
#include <iostream>
#include <vector>
#include <string.h>
int main(int argc, char* argv[]) {
    FastCDC fastcdc(64 * 1024, 256 * 1024, 512 * 1024);
    std::vector<Chunk> chunks;

    struct timespec tp1, tp2;

    int64_t start_time  = TimesUtil::getTimeNs();

    fastcdc.parse(argv[1], chunks);
    
    FILE* file = fopen(argv[1], "r");

    // test uint8_t to string
    // int offset = 0;
    // uint8_t buf[512 * 1024] = {0};
    // for (auto& chunk : chunks) {
    //     fread(buf, 1, chunks[0].getLength(), file);
    //     std::string t(buf, buf + chunks[0].getLength());
    //     uint8_t* px = (uint8_t*)t.c_str();
        
    //     offset += chunks[0].getLength();
    //     fseek(file, offset, SEEK_SET);
    // }

    // test bitmap
    // std::vector<Fringerprint> fps;
    // for (auto& chunk : chunks) {
    //     fps.emplace_back(chunk.getFringerprint());
    // }

    // FringerprintLRU lru(3000);
    
    // BitMap bitmap(2000);

    // std::cout << " ---- " << std::endl;
    // lru.put(fps);
    // std::cout << lru.getsize() << std::endl;
    // lru.get(fps, bitmap);


    int64_t end_time  = TimesUtil::getTimeNs();
    double time = (end_time - start_time) / 1e6;
    
    printf("\n======\n%.2fMB in %.2fMS (%.3f MB/S)\n", 1.0 * fastcdc.getPos() / (1024*1024), time, 1000.0 * fastcdc.getPos() /(time * 1024 * 1024));


    return 0;
}

