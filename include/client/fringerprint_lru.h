#ifndef FRINGERPRINT_LRU_H
#define FRINGERPRINT_LRU_H

#include "fringerprint.h"
#include "bitmap.h"
#include <vector>
#include <unordered_map>

// 定义LRU双向链表节点对象
class LRUNode {
public:
    LRUNode(Fringerprint key, LRUNode* next = nullptr, LRUNode* prev = nullptr) : key_(key), next_(next), prev_(prev) {};
    
    Fringerprint getKey() const;
    void setKey(Fringerprint key);

    LRUNode* getNext() const;
    void setNext(LRUNode* next);

    LRUNode* getPrev() const;
    void setPrev(LRUNode* prev);

private:
    Fringerprint key_;
    LRUNode* next_;
    LRUNode* prev_;
};


class FringerprintLRU {
public:
    FringerprintLRU(int capacity);

    bool get(std::vector<Fringerprint>& fps, BitMap& bitmap);
    bool put(std::vector<Fringerprint>& fps);

    int getsize() const;
    void setsize(int size);

    int getcapacity() const;
    void setcapacity(int capacity);

private:
    int size_;
    int capacity_;
    LRUNode* head_;
    LRUNode* tail_;
    std::unordered_map<Fringerprint, LRUNode*, Fringerprint> map_;
};

#endif
