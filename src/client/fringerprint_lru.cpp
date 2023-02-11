#include "fringerprint.h"
#include "fringerprint_lru.h"

#include <iostream>

template<class T>
T LRUNode<T>::getKey() const {
    return key_;
}

template<class T>
void LRUNode<T>::setKey(T key) {
    key_ = key;
}

template<class T>
LRUNode<T>* LRUNode<T>::getNext() const {
    return next_;
}

template<class T>
void LRUNode<T>::setNext(LRUNode* next) {
    next_ = next;
}

template<class T>
LRUNode<T>* LRUNode<T>::getPrev() const {
    return prev_;
}

template<class T>
void LRUNode<T>::setPrev(LRUNode* prev) {
    prev_ = prev;
}

FringerprintLRU::FringerprintLRU(int capacity) {
    size_ = 0;
    capacity_ = capacity;
    head_ = new LRUNode<std::string>("0");
    tail_ = new LRUNode<std::string>("0");
    head_->setNext(tail_);
    tail_->setPrev(head_);
}

bool FringerprintLRU::get(std::string fp) {
    if (map_.find(fp) == map_.end()) {
        return false;
    }

    LRUNode<std::string>* fpnode;
    fpnode = map_[fp];

    fpnode->getPrev()->setNext(fpnode->getNext());
    fpnode->getNext()->setPrev(fpnode->getPrev());

    fpnode->setNext(head_->getNext());
    head_->getNext()->setPrev(fpnode);
    fpnode->setPrev(head_);
    head_->setNext(fpnode);

    return true;
}

bool FringerprintLRU::put(std::string fp) {
    LRUNode<std::string>* fpnode;

    if (map_.find(fp) == map_.end()) {
        fpnode = new LRUNode<std::string>(fp);
        if (fpnode == nullptr) {
            return false;
        }
    } else {
        fpnode = map_[fp];
        fpnode->getPrev()->setNext(fpnode->getNext());
        fpnode->getNext()->setPrev(fpnode->getPrev());
        size_--;
    }

    if (size_ == capacity_) {
        LRUNode<std::string>* elimnode;
        elimnode = tail_->getPrev();
        elimnode->getPrev()->setNext(elimnode->getNext());
        elimnode->getNext()->setPrev(elimnode->getPrev());
        map_.erase(elimnode->getKey());
        size_--;
    }

    fpnode->setNext(head_->getNext());
    head_->getNext()->setPrev(fpnode);
    fpnode->setPrev(head_);
    head_->setNext(fpnode);

    map_.insert({fp, fpnode});
    size_++;

    return true;
}

int FringerprintLRU::getsize() const {
    return size_;
}

void FringerprintLRU::setsize(int size) {
    size_ = size;
}

int FringerprintLRU::getcapacity() const {
    return capacity_;
}
    
void FringerprintLRU::setcapacity(int capacity) {
    capacity_ = capacity;
}
