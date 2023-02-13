#include "bloomfilter.h"
#include "dbhelper.h"
#include "dbchunk.h"
#include "fringerprint.h"
#include "file_writer.h"
#include <iostream>

int main() {
    BloomFilter bloomfilter(BLOOMFILTER_SHIFT);
    
    Fringerprint fp;
    DBChunk chunk(1, 0, 1024 * 1024, 1), res;

    // DBHelper db("./db");
    // db.openDB();
    
    // db.put(fp, chunk);
    // if (db.get(fp, res)) {
    //     std::cout << res.getLength() << std::endl;
    // }

    // std::cout << db.size() << std::endl;
    // db.remove(fp);
    // std::cout << db.size() << std::endl;
    
    // db.closeDB();

    // FileWriter* filewriter = FileWriter::getInstance(64 * 1024 * 1024);
    // std::string s1 = "hello world";
    // std::string s2 = "come on lyq";
    // filewriter->write(s1, chunk);
    // filewriter->write(s2, chunk);
    // std::cout << chunk.getOffset() << " -- " << chunk.getLength() << std::endl;

    
    return 0;
}
