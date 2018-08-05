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
/* @file LRUCacheSegment.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the Time To Live Segment Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage of a segment with the behaviour of the TTL
 * Eviction Strategy
 *
 */
#include "TTLCacheSegment.h"
#include <string>
#include <map>

TTLCacheSegment::TTLCacheSegment(long long size, std::string category) {
    // TODO Auto-generated constructor stub
    setSize(size);
    setCategory(category);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
}

TTLCacheSegment::~TTLCacheSegment() {
    // TODO Auto-generated destructor stub
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 */
void TTLCacheSegment::resetRates() {
    this->writeOperation = 0;
    this->readOperation = 0;
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 * @todo Check if this should really be public
 */
void TTLCacheSegment::setSize(long long size) {
    this->cacheSize = size;
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long TTLCacheSegment::getSize() {
    return this->cacheSize;
}
/**
 * @brief Get the maximum size of the cache
 * @return Returns an integer Value describing the maximum size of the cache
 */
long long TTLCacheSegment::getMaxSize() {
    return this->maxCacheSize;
}
void TTLCacheSegment::periodicEvents() {
    purge();
}
/**
 * @brief deletes all Objects in the Cache
 */
void TTLCacheSegment::clearCache() {
    for (auto i : container) {
        delete i.second->second->first;
        delete i.second->second->second;
        delete i.second->second;
    }
    delete head;
}
/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool TTLCacheSegment::contains(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}
/**
 * @brief deletes video segment from the storage of the cache
 *
 * The only time this is called from outside is with MCD as admission strategy
 * @param id the video id of the video segment that has to be deleted
 */
void TTLCacheSegment::deleteSegment(std::string id) {
    if (container.find(id) != container.end()) {
        int freedSize = container[id]->second->first->getSize();
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
}
/**
 * @brief Inserts a video segment into the Cache
 *
 * A segment is inserted into the cache following the LFUDA algorithm
 * @param *pkg A VideoSegment
 */
std::list<std::string>* TTLCacheSegment::insertIntoCache(VideoSegment* pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    auto p = new std::pair<VideoSegment*, RecencyNode*>(pkg,
            new RecencyNode(keyBuilder, head, head->getNext()));
    auto k = new std::pair<double, std::pair<VideoSegment*, RecencyNode*>*>(
            omnetpp::simTime().dbl(), p);
    container[keyBuilder] = k;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    writeOperation++;
    cacheSize = cacheSize + pkg->getSize();
    return deletedVideoSegments;
}
/**
 * @brief reduces the size of the Cachesegment
 *
 * Cache Segments expand and reduce over time with the ACDC Eviction Strategy.
 * If the size of a cache segment is reduced, the video segments that do not fit
 * in it anymore are deleted. A list of the deleted video ids is returned
 *
 * @param size the size that is substracted from the subcache
 *
 * @return a list of video ids that were deleted when the cache size was reduced
 */
std::list<std::string>* TTLCacheSegment::reduce(int size) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - size) {
        toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    writeOperation++;
    return deletedVideoSegments;
}
/**
 * @brief Retrieves the video segment from the Cache. This should only be executed, if contains returns true.
 * @param rqst A segment request
 * @return The video segment that fullfills the segment request
 *
 */
VideoSegment* TTLCacheSegment::retrieveSegment(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->second->first;
    container[keyBuilder]->first = omnetpp::simTime().dbl();
    rearrangeCache(pkg);
    return pkg->dup();
}
/**
 * @brief expands the size of the cache segment
 *
 * with ACDC cache segements can be expanded to store more video segemnts
 *
 * @param i the size that is added to the maximum cache size
 *
 */
void TTLCacheSegment::expand(int i) {
    cacheSize = cacheSize + 1;
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int TTLCacheSegment::getReadOperations() {
    return this->readOperation;
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int TTLCacheSegment::getWriteOperations() {
    return this->writeOperation;
}
/*
 * @brief get the category of the cache segment
 * returns the category assigned to this cache segment
 *
 * @return the cateogry assigned to ths cache segment
 */
std::string TTLCacheSegment::getCategory() {
    return this->category;
}
/*
 * @brief rearranges the cache on hit in the TTL manner
 *
 * @param pkg the video segment that triggered the rearrangement
 */
void TTLCacheSegment::rearrangeCache(VideoSegment* pkg) {
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
/*
 * @brief Assignes a specific category to the cache
 * @param category the category of the elements that should be stored in this cache segemnt
 */
void TTLCacheSegment::setCategory(std::string category) {
    this->category = category;
}
/*
 * @brief deletes all expired elementes from the cache
 */
void TTLCacheSegment::purge() {
    for (auto i : container) {
        if ((omnetpp::simTime().dbl() - i.second->first) > timeToLive) {
            std::string toDelete = i.first;
            deleteSegment(toDelete);
        }
        readOperation++;
    }
}
