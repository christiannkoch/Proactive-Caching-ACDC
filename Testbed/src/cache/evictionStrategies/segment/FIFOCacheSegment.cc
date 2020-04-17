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
/* @file FIFOCacheSegment.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the First In First Out Segment Evictin Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage of a segment with the behaviour of the FIFO
 * Eviction Strategy
 *
 */
#include "FIFOCacheSegment.h"
#include "PointerAndCounter.h"
#include <sstream>

/*
 * @brief Creates a new FIFO CacheSegment
 * @param size The desired Size of this Cache
 * @param category the content category that is stored in this cache segment
 * @return A pointer to this class
 */
FIFOCacheSegment::FIFOCacheSegment(long long size, std::string category) {
    setSize(size);
    setCategory(category);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);

}

FIFOCacheSegment::~FIFOCacheSegment() {
}

void FIFOCacheSegment::periodicEvents() {

}

/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 */
void FIFOCacheSegment::resetRates() {
    this->readOperation = 0;
    this->writeOperation = 0;
}

/**
 * @brief deletes video segment from the storage of the cache
 *
 * The only time this is called from outside is with MCD as admission strategy
 * @param id the video id of the video segment that has to be deleted
 */
void FIFOCacheSegment::deleteSegment(std::string id) {
    if (container.find(id) != container.end()) {
        int freedSize = container[id]->first->getSegment()->getSize();
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
 * @return a list with all the ids of videos that were deleted
 */
std::list<std::string>* FIFOCacheSegment::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    auto p = new std::pair<PointerAndCounter*, RecencyNode*>(new PointerAndCounter(pkg, 0),
            new RecencyNode(keyBuilder, head, head->getNext()));
    container[keyBuilder] = p;
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
std::list<std::string>* FIFOCacheSegment::reduce(int size) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - size) {
        toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    maxCacheSize = maxCacheSize - size;
    writeOperation++;
    return deletedVideoSegments;
}

/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool FIFOCacheSegment::contains(SegmentRequest* rqst) {
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
VideoSegment* FIFOCacheSegment::retrieveSegment(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first->getSegment();
    container[keyBuilder]->first->increaseCount();
    rearrangeCache(pkg);
    return pkg->dup();
}

/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 * @todo Check if this should really be public
 */
void FIFOCacheSegment::setSize(long long size) {
    this->maxCacheSize = size;
}

/**
 * @brief Get the maximum size of the cache
 * @return Returns an integer Value describing the maximum size of the cache
 */
long long FIFOCacheSegment::getMaxSize(){
    return this->maxCacheSize;
}

/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long FIFOCacheSegment::getSize() {
    return this->cacheSize;
}

/**
 * @brief deletes all Objects in the Cache
 */
void FIFOCacheSegment::clearCache() {
    for (auto i : container) {
        delete i.second->first;
        delete i.second->second;
        delete i.second;
    }
    delete head;
}

/**
 * @brief is called upon a cache hit to rearrange the cache
 *
 * @param pkg the video segment which triggered the rearrangement
 * With FIFO nothing happens here
 */
void FIFOCacheSegment::rearrangeCache(VideoSegment *pkg) {

}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int FIFOCacheSegment::getReadOperations() {
    return this->readOperation;
}

/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int FIFOCacheSegment::getWriteOperations() {
    return this->writeOperation;
}

/**
 * @brief expands the size of the cache segment
 *
 * with ACDC cache segements can be expanded to store more video segemnts
 *
 * @param i the size that is added to the maximum cache size
 *
 */
void FIFOCacheSegment::expand(int i) {
    maxCacheSize = maxCacheSize + i;
}

/*
 * @brief Assignes a specific category to the cache
 * @param category the category of the elements that should be stored in this cache segemnt
 */
void FIFOCacheSegment::setCategory(std::string category) {
    this->category = category;
}
/*
 * @brief get the category of the cache segment
 * returns the category assigned to this cache segment
 *
 * @return the cateogry assigned to ths cache segment
 */
std::string FIFOCacheSegment::getCategory() {
    return this->category;
}

std::string FIFOCacheSegment::getCountsOfElements(){
    std::stringstream buf;
    for (auto i : container){
        buf << i.first << ", " << i.second->first->getCount() << "; ";
    }

    return buf.str();
}

