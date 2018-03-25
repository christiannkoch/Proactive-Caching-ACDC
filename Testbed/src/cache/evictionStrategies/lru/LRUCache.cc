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
/* @file ACDCCache.cc
 * @author Johannes Pfannmüller
 * @date
 * @version 1.0
 *
 * @brief Implements the Least Recently Used Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the LRU
 * Eviction Strategy
 *
 */
#include "LRUCache.h"
#include <list>
#include <string>
#include <vector>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

LRUCache::LRUCache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy) {
    // TODO Auto-generated constructor stub
    this->parameters = parameters;
    setSize(size);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
    expandStrat = storageAlterationStrategy;
    this->storageAlterations = storageAlterations;
    if (storageAlterations->size() > 0) {
        expanded = false;
    } else
        expanded = true;
}

LRUCache::~LRUCache() {
    delete storageAlterations;
}

void LRUCache::resetRates() {
    this->readOperation = 0;
    this->writeOperation = 0;
    if (!expanded
            and (omnetpp::simTime().dbl() > storageAlterations->begin()->first)) {
        alterCacheSize(storageAlterations->begin()->second);
        storageAlterations->erase(storageAlterations->begin());
        if (storageAlterations->size() == 0) {
            expanded = true;
        }
    }
}

void LRUCache::deletePackage(std::string id) {
    int freedSize = container[id]->first->getSize();
    RecencyNode* rec = container[id]->second;
    delete container[id]->first;
    rec->getPrev()->setNext(rec->getNext());
    rec->getNext()->setPrev(rec->getPrev());
    delete rec;
    delete container[id];
    container.erase(id);
    cacheSize = cacheSize - freedSize;
    writeOperation++;
}

void LRUCache::periodicEvents() {
}

void LRUCache::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize >= maxCacheSize - pkg->getSize()) {
        std::string toDelete = head->getPrev()->getValue();
        deletePackage(toDelete);
        //std::cout << "Insert while schleife vom Loeschen" <<"\n";
    }
    auto p = new std::pair<VideoSegment*, RecencyNode*>(pkg,
            new RecencyNode(keyBuilder, head, head->getNext()));
    container[keyBuilder] = p;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    cacheSize = cacheSize + pkg->getSize();

    writeOperation++;
}

bool LRUCache::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}

VideoSegment *LRUCache::retrievePackage(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
    rearrangeCache(pkg);
    return pkg->dup();
}

void LRUCache::setSize(long long size) {
    this->maxCacheSize = size;
}

long long LRUCache::getSize() {
    return this->cacheSize;
}

void LRUCache::clearCache() {
    for (auto i : container) {
        delete i.second->first;
        delete i.second->second;
        delete i.second;
    }
    delete head;
}

void LRUCache::rearrangeCache(VideoSegment *pkg) {
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

void LRUCache::alterCacheSize(double newCacheSize) {
    maxCacheSize = newCacheSize;
}

int LRUCache::getWriteOperations() {
    return this->writeOperation;
}

int LRUCache::getReadOperations() {
    return this->readOperation;
}
