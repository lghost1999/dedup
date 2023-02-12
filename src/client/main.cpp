#include "fastcdc.h"
#include "chunk.h"
#include "fringerprint_lru.h"

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

    std::cout << chunks.size() << std::endl;

    FringerprintLRU lru(3000);
    for (int i = 0; i < chunks.size(); i++) {
        lru.put(chunks[i].getFringerprint());
    }
    std::cout << lru.getsize() << std::endl;

    int cnt = 0;
    for (int i = 0; i < chunks.size(); i++) {
        if (lru.get(chunks[i].getFringerprint())) {
            cnt++;
        }
    }

    std::cout << cnt << std::endl;


    clock_gettime(CLOCK_MONOTONIC, &tp2);
    double elapsed = (tp2.tv_sec - tp1.tv_sec)*1000 + (tp2.tv_nsec - tp1.tv_nsec)/1000000.0;
    printf("\n======\n%.2fMB in %.2fMS (%.3f MB/S)\n", 1.0 * fastcdc.getPos()/(1024*1024), elapsed, 1000.0*fastcdc.getPos()/(elapsed*1024*1024));










    // std::string str[4] = {"3b81b48641833cadab33590859f19cc3", 
    //                       "894c5451bcc2af7bdf77fd1666638599",
    //                       "948a22b341bc5fa059a120fc471fd3e5",
    //                       "f7246edd358afed292b16906aa842487"};

    // for (int i = 0; i < 4; i++) {
    //     for (int j = 0; j < chunks.size(); j++) {
    //         if (chunks[j].getFringerprint().val() == str[i]) {
    //             std::cout << j << " : " << chunks[j].getOffset() << " : " << chunks[j].getLength() <<
    //                          " : " << chunks[j].getFringerprint().val() << "------";
    //         }
    //     }
    //     std::cout << std::endl;
    // }

    return 0;
}


// 3b81b48641833cadab33590859f19cc3
// 894c5451bcc2af7bdf77fd1666638599
// 948a22b341bc5fa059a120fc471fd3e5
// f7246edd358afed292b16906aa842487
