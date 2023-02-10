#include "fastcdc.h"
#include "chunk.h"

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    FastCDC fastcdc(64 * 1024, 256 * 1024, 512 * 1024);
    std::vector<Chunk> chunks;
    fastcdc.parse(argv[1], chunks);

    for (auto& chunk : chunks) {
        std::cout << chunk.getPath() << " : " << chunk.getOffset() << " : " << chunk.getLength() <<
                     chunk.getRefcnt() << " : " << chunk.getFringerprint().val() << std::endl;
    }
    return 0;
}
