#ifndef DBHELPER_H
#define DBHELPER_H

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
#include <iostream>

#include "dbchunk.h"
#include "fringerprint.h"

class DBHelper {
public:
    DBHelper(std::string path) : path_(path) {};

    bool openDB();
    bool closeDB();

    template <typename K, typename V>
    bool get(K& key, V& val);

    template <typename K, typename V>
    bool put(K& key, V& val);

    template <typename K>
    bool remove(K& key);

    size_t size();

private:
    bool get(const void* key, size_t key_size, std::string& val);
    bool put(const void* key, size_t key_size, const void* val, size_t val_size);
    bool remove(const void* key, size_t key_size);

    std::string path_;
    rocksdb::DB *chunkDB_;
    rocksdb::Options options_;
};

#endif
