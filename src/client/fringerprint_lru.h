#ifndef FRINGERPRINT_LRU_H
#define FRINGERPRINT_LRU_H

#include "fringerprint.h"
#include <unordered_map>

template<class T>
class LRUNode {
public:
    LRUNode(T key, LRUNode* next = nullptr, LRUNode* prev = nullptr) : key_(key), next_(next), prev_(prev) {};
    
    T getKey() const;
    void setKey(T key);

    LRUNode* getNext() const;
    void setNext(LRUNode* next);

    LRUNode* getPrev() const;
    void setPrev(LRUNode* prev);

private:
    T key_;
    LRUNode* next_;
    LRUNode* prev_;
};


class FringerprintLRU {
public:
    FringerprintLRU(int capacity);

    bool get(std::string fp);
    bool put(std::string fp);

    int getsize() const;
    void setsize(int size);

    int getcapacity() const;
    void setcapacity(int capacity);

private:
    int size_;
    int capacity_;
    LRUNode<std::string>* head_;
    LRUNode<std::string>* tail_;
    std::unordered_map<std::string, LRUNode<std::string>*> map_;
};

#endif
