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
 * @brief Implements the Random Segment Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage of a segment with the behaviour of the RAND
 * Eviction Strategy
 *
 */
#include "RANDCacheSegment.h"
#include <algorithm>

/*
 * @brief Creates a new Rand CacheSegment
 * @param size The desired Size of this Cache
 * @param category the content category that is stored in this cache segment
 * @return A pointer to this class
 */
RANDCacheSegment::RANDCacheSegment(long long size, std::string category) {
    // TODO Auto-generated constructor stub
    setSize(size);
    setCategory(category);
}

RANDCacheSegment::~RANDCacheSegment() {
    // TODO Auto-generated destructor stub
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 * @todo Check if this should really be public
 */
void RANDCacheSegment::setSize(long long size) {
    this->maxCacheSize = size;
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long RANDCacheSegment::getSize() {
    return this->cacheSize;
}
/**
 * @brief Get the maximum size of the cache
 * @return Returns an integer Value describing the maximum size of the cache
 */
long long RANDCacheSegment::getMaxSize(){
    return this->maxCacheSize;
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 */
void RANDCacheSegment::resetRates() {
    this->writeOperation = 0;
    this->readOperation = 0;
}

void RANDCacheSegment::periodicEvents() {

}
/**
 * @brief deletes all Objects in the Cache
 */
void RANDCacheSegment::clearCache() {
    for (auto i : container) {
        delete i.second;
    }
}
/**
 * @brief deletes video segment from the storage of the cache
 *
 * The only time this is called from outside is with MCD as admission strategy
 * @param id the video id of the video segment that has to be deleted
 */
void RANDCacheSegment::deleteSegment(std::string id) {
    if (container.find(id) != container.end()) {
        int freedSize = container[id]->getSize();
        cacheSize = cacheSize - freedSize;
        delete container[id];
        container.erase(id);
        keyList.erase(std::remove(keyList.begin(), keyList.end(), id),
                keyList.end());
    }
}
/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool RANDCacheSegment::contains(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}

/**
 * @brief Inserts a video segment into the Cache
 *
 * A segment is inserted into the cache following the RAND algorithm
 * @param *pkg A VideoSegment
 * @return a list with all the ids of videos that were deleted
 */
std::list<std::string>* RANDCacheSegment::insertIntoCache(VideoSegment* pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize >= maxCacheSize - pkg->getSize()) {
        int random = rand() % container.size();
        toDelete = keyList[random];
        deleteSegment(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    container[keyBuilder] = pkg;
    keyList.push_back(keyBuilder);
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
std::list<std::string>* RANDCacheSegment::reduce(int size) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - size) {
        int random = rand() % container.size();
        toDelete = keyList[random];
        deleteSegment(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    maxCacheSize = maxCacheSize - size;
    writeOperation++;
    return deletedVideoSegments;
}
/**
 * @brief Retrieves the video segment from the Cache. This should only be executed, if contains returns true.
 * @param rqst A segment request
 * @return The video segment that fullfills the segment request
 *
 */
VideoSegment* RANDCacheSegment::retrieveSegment(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    return container[keyBuilder]->dup();
}
/**
 * @brief expands the size of the cache segment
 *
 * with ACDC cache segements can be expanded to store more video segemnts
 *
 * @param i the size that is added to the maximum cache size
 *
 */
void RANDCacheSegment::expand(int i) {
    cacheSize = cacheSize + i;
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int RANDCacheSegment::getReadOperations() {
    return this->readOperation;
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int RANDCacheSegment::getWriteOperations() {
    return this->writeOperation;
}
/*
 * @brief get the category of the cache segment
 * returns the category assigned to this cache segment
 *
 * @return the cateogry assigned to ths cache segment
 */
std::string RANDCacheSegment::getCategory() {
    return this->category;
}
/**
 * @brief is called upon a cache hit to rearrange the cache
 *
 * @param pkg the video segment which triggered the rearrangement
 * With RAND nothing happens here
 */
void RANDCacheSegment::rearrangeCache(VideoSegment* pkg) {

}
/*
 * @brief Assignes a specific category to the cache
 * @param category the category of the elements that should be stored in this cache segemnt
 */
void RANDCacheSegment::setCategory(std::string category) {
    this->category = category;
}
