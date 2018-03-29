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
/* @file FIFOCache.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the First In First Out Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the FIFO
 * Eviction Strategy
 *
 */
#include "FIFOCache.h"
/*
 * @brief Creates a new FIFO Cache for caching functionalities
 * @param size The desired Size of this Cache
 * @param storageAlterations a vector of pairs of double value representing storage alterations
 * @param storageAlterationStrategy the strategy used when altering the size of the storage
 * @return A pointer to this class
 */
FIFOCache::FIFOCache(std::vector<std::string>* parameters, long long size,
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

FIFOCache::~FIFOCache() {
    delete storageAlterations;
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 * Also triggers the storage alteration if any exist
 */
void FIFOCache::resetRates() {
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
 * The only time this is called from outside is with MCD as admission strategy
 * @param id the video id of the video segment that has to be deleted
 */
void FIFOCache::deleteSegment(std::string id) {
    int freedSize = container[id]->first->getSize();
    delete container[id]->first;
    RecencyNode* rec = container[id]->second;
    rec->getPrev()->setNext(rec->getNext());
    rec->getNext()->setPrev(rec->getPrev());
    delete rec;
    delete container[id];
    container.erase(id);
    cacheSize = cacheSize - freedSize;
    writeOperation++;
}
/**
 * @brief executes periodic events
 *
 * periodic events can regularily check on something
 */
void FIFOCache::periodicEvents() {
}
/**
 * @brief Inserts a VideoSegment into the Cache
 *
 * We first check, if there is enough space in the cache to insert the video segment.
 * If not, the video segements that are the oldest get deleted until enough space is freed up.
 * When a video segment is inserted into the cache, we create a new FrequencyNode which holds the
 * video id. The node points with the next pointer to the previous first element in the FrequencyQueue
 * and with the previous pointer to the end of the FrequencyQueue.
 *
 * @param *pkg A VideoSegment
 */
void FIFOCache::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        std::string toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
    }
    auto p = new std::pair<VideoSegment*, RecencyNode*>(pkg,
            new RecencyNode(keyBuilder, head, head->getNext()));
    container[keyBuilder] = p;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
}
/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool FIFOCache::contains(SegmentRequest *rqst) {
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
 * @return The video segment that fullfills the segment request
 *
 */
VideoSegment* FIFOCache::retrieveSegment(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
    return pkg->dup();
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 * @todo Check if this should really be public
 */
void FIFOCache::setSize(long long size) {
    this->maxCacheSize = size;
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 * @todo Do I need this?
 */
long long FIFOCache::getSize() {
    return this->maxCacheSize;
}
/**
 * @brief deletes all Objects in the Cache
 */
void FIFOCache::clearCache() {
    for (auto i : container) {
        delete i.second->first;
        delete i.second->second;
        delete i.second;
    }
    delete head;
}
/**
 * @brief alters the cache size
 *
 * a function that alters the maximum size of the cache to a given double value
 *
 * @param newCacheSize a double value representing the new size of the cache
 */
void FIFOCache::alterCacheSize(double newCacheSize) {
    maxCacheSize = newCacheSize;
}
/**
 * @brief is called upon a cache hit to rearrange the cache
 *
 * @param pkg the video segment which triggered the rearrangement
 * With FIFO nothing happens here
 */
void FIFOCache::rearrangeCache(VideoSegment *pkg) {

}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int FIFOCache::getReadOperations() {
    return this->readOperation;
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int FIFOCache::getWriteOperations() {
    return this->writeOperation;
}
