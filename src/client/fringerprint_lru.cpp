#include "fringerprint.h"
#include "fringerprint_lru.h"

#include <iostream>

Fringerprint LRUNode::getKey() const {
    return key_;
}

void LRUNode::setKey(Fringerprint key) {
    key_ = key;
}

LRUNode* LRUNode::getNext() const {
    return next_;
}

void LRUNode::setNext(LRUNode* next) {
    next_ = next;
}

LRUNode* LRUNode::getPrev() const {
    return prev_;
}

void LRUNode::setPrev(LRUNode* prev) {
    prev_ = prev;
}

FringerprintLRU::FringerprintLRU(int capacity) {
    size_ = 0;
    capacity_ = capacity;
    head_ = new LRUNode(Fringerprint::zero);
    tail_ = new LRUNode(Fringerprint::zero);
    head_->setNext(tail_);
    tail_->setPrev(head_);
    map_.clear();
}

bool FringerprintLRU::get(std::vector<Fringerprint>& fps, BitMap& bitmap) {
    for (size_t i = 0; i < fps.size(); i++) {
        if (map_.find(fps[i]) == map_.end()) {
            continue;
        }

        LRUNode* fpnode;
        fpnode = map_[fps[i]];

        fpnode->getPrev()->setNext(fpnode->getNext());
        fpnode->getNext()->setPrev(fpnode->getPrev());

        fpnode->setNext(head_->getNext());
        head_->getNext()->setPrev(fpnode);
        fpnode->setPrev(head_);
        head_->setNext(fpnode);

        bitmap.put(i);
    }
    
    return true;
}

bool FringerprintLRU::put(std::vector<Fringerprint>& fps) {
    LRUNode* fpnode;

    for (size_t i = 0; i < fps.size(); i++) {
        if (map_.find(fps[i]) == map_.end()) {
            fpnode = new LRUNode(fps[i]);
            if (fpnode == nullptr) {
                return false;
            }
        } else {
            fpnode = map_[fps[i]];
            fpnode->getPrev()->setNext(fpnode->getNext());
            fpnode->getNext()->setPrev(fpnode->getPrev());
            size_--;
        }

        if (size_ == capacity_) {
            LRUNode* elimnode;
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

        map_.insert({fps[i], fpnode});
        size_++;
    }
    
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
