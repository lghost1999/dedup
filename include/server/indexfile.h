#ifndef INDEXFILE_H
#define INDEXFILE_H

#include "fringerprint.h"
#include <string>
#include <vector>
#include <dbchunk.h>

class IndexFile {
public:
    static bool writeFringerprints(std::string& dir_name, std::string& file_name, std::vector<Fringerprint>& fps);

    static bool readChunks(std::string& dir_name, std::string& file_name, std::vector<DBChunk>& dbchunks);
};

#endif
