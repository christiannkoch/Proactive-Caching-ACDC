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
 * @brief Implements the Time To Live eviction strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the Time To Live
 * Eviction Strategy
 *
 */
#include "TTLCache.h"
#include <list>
#include <string>
#include <vector>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

TTLCache::TTLCache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy) {
    this->parameters = parameters;
    this->timeToLive = std::stoi(parameters->at(0));
    expandStrat = storageAlterationStrategy;
    this->storageAlterations = storageAlterations;
    if (storageAlterations->size() > 0) {
        expanded = false;
    } else
        expanded = true;
    setSize(size);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
}

TTLCache::~TTLCache() {
    delete storageAlterations;
}

void TTLCache::periodicEvents() {
    purge();
}

void TTLCache::resetRates() {
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

void TTLCache::deletePackage(std::string id) {
    int freedSize = container[id]->second->first->getSize();
    RecencyNode* rec = container[id]->second->second;
    delete container[id]->second->first;
    rec->getPrev()->setNext(rec->getNext());
    rec->getNext()->setPrev(rec->getPrev());
    delete rec;
    delete container[id]->second;
    delete container[id];
    container.erase(id);
    cacheSize = cacheSize - freedSize;
    writeOperation++;
}

void TTLCache::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        std::string toDelete = head->getPrev()->getValue();
        deletePackage(toDelete);
    }
    auto p = new std::pair<VideoSegment*, RecencyNode*>(pkg,
            new RecencyNode(keyBuilder, head, head->getNext()));
    auto k = new std::pair<double, std::pair<VideoSegment*, RecencyNode*>*>(
            omnetpp::simTime().dbl(), p);
    container[keyBuilder] = k;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
}

void TTLCache::purge() {
    for (auto i : container) {
        if ((omnetpp::simTime().dbl() - i.second->first) > timeToLive) {
            std::string toDelete = i.first;
            deletePackage(toDelete);
        }
        readOperation++;
    }

}

bool TTLCache::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}
VideoSegment* TTLCache::retrievePackage(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->second->first;
    container[keyBuilder]->first = omnetpp::simTime().dbl();
    rearrangeCache(pkg);
    return pkg->dup();
}
long long TTLCache::getSize() {
    return this->cacheSize;
}
void TTLCache::clearCache() {
    for (auto i : container) {
        delete i.second->second->first;
        delete i.second->second->second;
        delete i.second->second;
    }
    delete head;
}
int TTLCache::getWriteOperations() {
    return this->writeOperation;
}
int TTLCache::getReadOperations() {
    return this->readOperation;
}
void TTLCache::setSize(long long size) {
    cacheSize = size;
}
void TTLCache::rearrangeCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    RecencyNode* rec = container[keyBuilder]->second->second;
    RecencyNode* next_rec = rec->getNext();
    rec->getPrev()->setNext(next_rec);
    next_rec->setPrev(rec->getPrev());
    rec->setNext(head->getNext());
    rec->getNext()->setPrev(rec);
    rec->setPrev(head);
    head->setNext(rec);
}

void TTLCache::alterCacheSize(double newCacheSize) {
    maxCacheSize = newCacheSize;
}
