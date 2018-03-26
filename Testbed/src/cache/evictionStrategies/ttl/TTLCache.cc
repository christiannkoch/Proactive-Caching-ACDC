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
/* @file TTLCache.cc
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
/*
 * @brief Creates a new TTLCache for caching functionalities
 * @param parameters the parameters for this eviction strategy
 * @param size The desired Size of this Cache
 * @param storageAlterations a vector of pairs of double value representing storage alterations
 * @param storageAlterationStrategy the strategy used when altering the size of the storage
 * @return A pointer to this class
 */
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
/**
 * @brief executes periodic events
 *
 * periodic events can regularily check on something
 */
void TTLCache::periodicEvents() {
    purge();
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 * Also triggers the storage alteration if any exist
 */
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
/**
 * @brief deletes video segment from the storage of the cache
 *
 * When a video segment is deleted, we check if the corresponding recency node
 * is now empty. if so, we update the pointers and delete the recency node in
 * order to save storage.
 * @param id the video id of the video segment that has to be deleted
 */
void TTLCache::deleteSegment(std::string id) {
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
/**
 * @brief Inserts a video segment into the Cache
 *
 * A segment is inserted into the cache following the LFUDA algorithm
 * @param *pkg A VideoSegment
 */
void TTLCache::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        std::string toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
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
/*
 * @brief deletes all expired elementes from the cache
 */
void TTLCache::purge() {
    for (auto i : container) {
        if ((omnetpp::simTime().dbl() - i.second->first) > timeToLive) {
            std::string toDelete = i.first;
            deleteSegment(toDelete);
        }
        readOperation++;
    }

}
/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool TTLCache::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}
/**
 * @brief Retrieves the video segment from the Cache. This should only be executed, if contains returns true.
 * @param rqst A segment request
 *
 * The video segment is returned and we call rearrangeCache.
 * @return The video segment that fullfills the segment request
 *
 */
VideoSegment* TTLCache::retrieveSegment(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->second->first;
    container[keyBuilder]->first = omnetpp::simTime().dbl();
    rearrangeCache(pkg);
    return pkg->dup();
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long TTLCache::getSize() {
    return this->cacheSize;
}
/**
 * @brief deletes all Objects in the Cache
 */
void TTLCache::clearCache() {
    for (auto i : container) {
        delete i.second->second->first;
        delete i.second->second->second;
        delete i.second->second;
    }
    delete head;
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int TTLCache::getWriteOperations() {
    return this->writeOperation;
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int TTLCache::getReadOperations() {
    return this->readOperation;
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 */
void TTLCache::setSize(long long size) {
    cacheSize = size;
}

/*
 * @brief rearranges the cache on hit in the TTL manner
 *
 * @param pkg the video segment that triggered the rearrangement
 */
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
/**
 * @brief alters the cache size
 *
 * a function that alters the maximum size of the cache to a given double value
 *
 * @param newCacheSize a double value representing the new size of the cache
 */
void TTLCache::alterCacheSize(double newCacheSize) {
    maxCacheSize = newCacheSize;
}
