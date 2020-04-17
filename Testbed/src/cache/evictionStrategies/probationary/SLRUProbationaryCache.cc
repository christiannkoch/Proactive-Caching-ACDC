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
/* @file SLRUProbationaryCache.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the probationary cache used in SLRU
 *
 * @section DESCRIPTION
 *
 * since slru uses a probationary cache that can insert an element at the end of the cache
 * the ARCProbationaryCache could not be used.
 */
#include "SLRUProbationaryCache.h"
#include <list>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "PointerAndCounter.h"
#include <sstream>

/*
 * @brief Creates a new ARCProbationaryCache
 * @param size the size of the cache
 * @return A pointer to this class
 */
SLRUProbationaryCache::SLRUProbationaryCache(long long size) {
    setSize(size);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
}

SLRUProbationaryCache::~SLRUProbationaryCache() {
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 */
void SLRUProbationaryCache::resetRates() {
    this->readOperation = 0;
    this->writeOperation = 0;
}
/**
 * @brief executes periodic events
 *
 * periodic events can regularily check on something
 */
void SLRUProbationaryCache::periodicEvents() {

}
/**
 * @brief deletes video segment from the storage of the cache
 *
 * When a video segment is deleted, we check if the corresponding recency node
 * is now empty. if so, we update the pointers and delete the recency node in
 * order to save storage.
 * @param id the video id of the video segment that has to be deleted
 */
void SLRUProbationaryCache::deleteSegment(std::string id) {
    if (container.find(id) != container.end()) {
        int freedSize = container[id]->first->getSegment()->getSize();
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

}
/**
 * @brief Inserts a video segment into the Cache
 *
 * A segment is inserted into the cache following SLRUProbationaryCache algorithm
 * @param *pkg A VideoSegment
 * @return a list of video ids that were deleted in order to insert the segment
 */
std::list<std::string>* SLRUProbationaryCache::insertIntoCache(
        VideoSegment *pkg) {

    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize >= maxCacheSize - pkg->getSize()) {
        std::string toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
    }
    auto p = new std::pair<PointerAndCounter*, RecencyNode*>(new PointerAndCounter(pkg, 0),
            new RecencyNode(keyBuilder, head, head->getNext()));
    container[keyBuilder] = p;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
    return nullptr;
}
/*
 * @brief deletes a video segment
 */
void SLRUProbationaryCache::drop(VideoSegment* pkg) {
    std::string toDelete = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    deleteSegment(toDelete);
}
/*
 * @brief inserts a video segment at the end of the cache
 * @param pkg the video segment to insert at the end of the cache
 */
void SLRUProbationaryCache::insertIntoCacheAtEnd(VideoSegment* pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        std::string toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
    }
    auto p = new std::pair<PointerAndCounter*, RecencyNode*>(new PointerAndCounter(pkg, 0),
            new RecencyNode(keyBuilder, head->getPrev(), head));
    container[keyBuilder] = p;
    head->getPrev()->setNext(p->second);
    head->setPrev(p->second);
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
}
/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool SLRUProbationaryCache::contains(SegmentRequest* rqst) {
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
VideoSegment* SLRUProbationaryCache::retrieveSegment(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first->getSegment();
    container[keyBuilder]->first->increaseCount();
    return pkg->dup();
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 */
void SLRUProbationaryCache::setSize(long long size) {
    this->maxCacheSize = size;
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long SLRUProbationaryCache::getSize() {
    return this->cacheSize;
}
/**
 * @brief Get the maximum size of the cache
 * @return Returns an integer Value describing the maximum size of the cache
 */
long long SLRUProbationaryCache::getMaxSize() {
    return this->maxCacheSize;
}
/**
 * @brief deletes all Objects in the Cache
 */
void SLRUProbationaryCache::clearCache() {
    for (auto i : container) {
        delete i.second->first;
        delete i.second->second;
        delete i.second;
    }
    delete head;
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int SLRUProbationaryCache::getReadOperations() {
    return this->readOperation;
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int SLRUProbationaryCache::getWriteOperations() {
    return this->writeOperation;
}

std::string SLRUProbationaryCache::getCountsOfElements(){
    std::stringstream buf;
    for (auto i : container){
        buf << i.first << ", " << i.second->first->getCount() << "; ";
    }

    return buf.str();
}

