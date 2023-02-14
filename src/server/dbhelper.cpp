#include <butil/logging.h>

#include "dbhelper.h"

bool DBHelper::openDB() {
    options_.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options_, path_, &chunkDB_);

    if (!status.ok()) {
        LOG(ERROR) << status.ToString();
        return false;
    }

    return true;
}
bool DBHelper::closeDB() {
    if (chunkDB_ == nullptr) {
        LOG(ERROR) << "RocksDB close error";
        return false;
    }

    delete chunkDB_;
    return true;
}

template <typename K, typename V>
bool DBHelper::get(K& key, V& val) {
    std::string value;

    if (!get((const void*)&key, sizeof(key), value)) {
        return false;
    }

    memcpy(&val, value.data(), value.size());

    return true;    
}

template <typename K, typename V>
bool DBHelper::put(K& key, V& val) {
    return put(&key, sizeof(key), &val, sizeof(val));
}

template <typename K>
bool DBHelper::remove(K& key)  {
    return remove(&key, sizeof(key));
}

bool DBHelper::get(const void* key, size_t key_size, std::string& val) {
    rocksdb::Status s = chunkDB_->Get(rocksdb::ReadOptions(), 
                                      rocksdb::Slice((const char *) key, key_size), 
                                      &val);
    if (!s.ok()) {
        LOG(ERROR) << "RocksDB read key error";
        return false;
    }

    return true;
}

bool DBHelper::put(const void* key, size_t key_size, const void* val, size_t val_size) {
    rocksdb::Status s = chunkDB_->Put(rocksdb::WriteOptions(),
                                      rocksdb::Slice((const char *) key, key_size),
                                      rocksdb::Slice((const char *) val, val_size));

    if (!s.ok()) {
        LOG(ERROR) << "RocksDB write key error";
        return false;
    }

    return true;
}

bool DBHelper::remove(const void* key, size_t key_size) {
    rocksdb::Status s = chunkDB_->Delete(rocksdb::WriteOptions(),
                                         rocksdb::Slice((const char *) key, key_size));
    
    if (!s.ok()) {
        LOG(ERROR) << "RocksDB delete key error";
        return false;
    }

    return true;
}

size_t DBHelper::size() {
    int cnt = 0;
    rocksdb::Iterator* it = chunkDB_->NewIterator(rocksdb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        Fringerprint fp;
        memcpy(&fp, it->key().ToString().data(), 16);
        std::cout << fp.val() << ": "  << it->value().ToString() << std::endl;
        cnt++;
    }
    assert(it->status().ok()); 
    delete it;

    return cnt;
}

// 模板不支持分离编译, 需要将函数模板先实例化
// 在分离式编译的环境下，编译器编译某一个.cpp文件时并不知道另一个.cpp文件的存在，也不会去查找，连接器确定函数真实调用地址
// 模板仅在运行时才实例化，连接器无法确定真实地址
template bool DBHelper::get<Fringerprint, DBChunk>(Fringerprint&, DBChunk&);
template bool DBHelper::put<Fringerprint, DBChunk>(Fringerprint&, DBChunk&);
template bool DBHelper::remove<Fringerprint>(Fringerprint&);
