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
#include "ARCProbationaryCache.h"
#include <omnetpp.h>
#include <list>
#include <string>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
/*
 * @brief Creates a new ARCProbationaryCache
 * @param size the size of the cache
 * @return A pointer to this class
 */
ARCProbationaryCache::ARCProbationaryCache(long long size) {
    setSize(size);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
}

ARCProbationaryCache::~ARCProbationaryCache() {
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 */
void ARCProbationaryCache::resetRates() {
    this->readOperation = 0;
    this->writeOperation = 0;
}
/**
 * @brief deletes video segment from the storage of the cache
 *
 * When a video segment is deleted, we check if the corresponding recency node
 * is now empty. if so, we update the pointers and delete the recency node in
 * order to save storage.
 * @param id the video id of the video segment that has to be deleted
 */
void ARCProbationaryCache::deleteSegment(std::string id) {
    if (container.find(id) != container.end()) {
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
}
/**
 * @brief Inserts a video segment into the Cache
 *
 * A segment is inserted into the cache following ARCProbationaryCache algorithm
 * @param *pkg A VideoSegment
 * @return a list of video ids that were deleted in order to insert the segment
 */
std::list<std::string>* ARCProbationaryCache::insertIntoCache(
        VideoSegment *pkg) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    auto p = new std::pair<VideoSegment*, RecencyNode*>(pkg,
            new RecencyNode(keyBuilder, head, head->getNext()));
    container[keyBuilder] = p;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
    return deletedVideoSegments;

}
/*
 * @brief reduces the maximum size of the cache
 * @param size the size that is substracted from the maximum size
 * @return a list of video ids that were deltetd when the maximum size was altered.
 */
std::list<std::string>* ARCProbationaryCache::reduce(int size) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    maxCacheSize = maxCacheSize - size;
    while (cacheSize > maxCacheSize - size) {
        toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    writeOperation++;
    return deletedVideoSegments;
}
/*
 * @brief deletes a video segment
 */
void ARCProbationaryCache::drop(VideoSegment* pkg) {
    std::string toDelete = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    deleteSegment(toDelete);
}

/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 */
void ARCProbationaryCache::setSize(long long size) {
    this->maxCacheSize = size;
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long ARCProbationaryCache::getSize() {
    return this->cacheSize;
}
/**
 * @brief Get the maximum size of the cache
 * @return Returns an integer Value describing the maximum size of the cache
 */
long long ARCProbationaryCache::getMaxSize() {
    return this->maxCacheSize;
}
/**
 * @brief Retrieves the video segment from the Cache. This should only be executed, if contains returns true.
 * @param rqst A segment request
 *
 * The video segment is returned and we call rearrangeCache.
 * @return The video segment that fullfills the segment request
 *
 */
VideoSegment* ARCProbationaryCache::retrieveSegment(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
    return pkg->dup();
}
/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool ARCProbationaryCache::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}
/**
 * @brief executes periodic events
 *
 * periodic events can regularily check on something
 */
void ARCProbationaryCache::periodicEvents() {
}
/**
 * @brief deletes all Objects in the Cache
 */
void ARCProbationaryCache::clearCache() {
    for (auto i : container) {
        delete i.second->first;
        delete i.second->second;
        delete i.second;
    }
    delete head;
}
/*
 * @brief expands the maximum cache storage
 * @param i the value that is added to the maximum cache storage
 */
void ARCProbationaryCache::expand(int i) {
    maxCacheSize = maxCacheSize + i;
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int ARCProbationaryCache::getReadOperations() {
    return this->readOperation;
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int ARCProbationaryCache::getWriteOperations() {
    return this->writeOperation;
}
