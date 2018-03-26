//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "LRUCacheSegment.h"
#include <omnetpp.h>
#include <list>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

LRUCacheSegment::LRUCacheSegment(long long size, std::string category) {
    setSize(size);
    setCategory(category);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
}

LRUCacheSegment::~LRUCacheSegment() {
}

void LRUCacheSegment::resetRates() {
    this->writeOperation = 0;
    this->readOperation = 0;
}

void LRUCacheSegment::periodicEvents() {
}

VideoSegment* LRUCacheSegment::getElementWithSecondChance() {
    if (container.size() > 1) {
        writeOperation++;
        return container[head->getPrev()->getValue()]->first->dup();
    }

    else
        return nullptr;
}

std::list<std::string>* LRUCacheSegment::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        toDelete = head->getPrev()->getValue();
        deletePackage(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    auto p = new std::pair<VideoSegment*, RecencyNode*>(pkg,
            new RecencyNode(keyBuilder, head, head->getNext()));
    container[keyBuilder] = p;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    writeOperation++;
    cacheSize = cacheSize + pkg->getSize();
    return deletedVideoSegments;
}

std::list<std::string>* LRUCacheSegment::reduce(int size) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    maxCacheSize = maxCacheSize - size;
    while (cacheSize > maxCacheSize - size) {
        toDelete = head->getPrev()->getValue();
        deletePackage(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    writeOperation++;
    return deletedVideoSegments;
}

void LRUCacheSegment::deletePackage(std::string id) {
    if (container.find(id) != container.end()) {
        int freedSize = container[id]->first->getSize();
        RecencyNode* rec = container[id]->second;
        rec->getPrev()->setNext(rec->getNext());
        rec->getNext()->setPrev(rec->getPrev());
        delete container[id]->first;
        delete rec;
        delete container[id];
        container.erase(id);
        cacheSize = cacheSize - freedSize;
        writeOperation++;

    }
}

bool LRUCacheSegment::contains(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}

VideoSegment* LRUCacheSegment::retrieveSegment(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
    rearrangeCache(pkg);
    return pkg->dup();
}

void LRUCacheSegment::setSize(long long size) {
    this->maxCacheSize = size;
}

long long LRUCacheSegment::getSize() {
    return this->cacheSize;
}

long long LRUCacheSegment::getMaxSize(){
    return this->maxCacheSize;
}

void LRUCacheSegment::clearCache() {
    for (auto i : container) {
        delete i.second->first;
        delete i.second->second;
        delete i.second;
    }
    delete head;
}

void LRUCacheSegment::rearrangeCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    RecencyNode* rec = container[keyBuilder]->second;
    RecencyNode* next_rec = rec->getNext();

    rec->getPrev()->setNext(next_rec);
    next_rec->setPrev(rec->getPrev());
    rec->setNext(head->getNext());
    rec->getNext()->setPrev(rec);
    rec->setPrev(head);
    head->setNext(rec);
}

int LRUCacheSegment::getReadOperations() {
    return this->readOperation;
}
int LRUCacheSegment::getWriteOperations() {
    return this->writeOperation;
}

void LRUCacheSegment::expand(int i) {
    maxCacheSize = maxCacheSize + i;
}

void LRUCacheSegment::setCategory(std::string category) {
    this->category = category;
}

std::string LRUCacheSegment::getCategory() {
    return this->category;
}
