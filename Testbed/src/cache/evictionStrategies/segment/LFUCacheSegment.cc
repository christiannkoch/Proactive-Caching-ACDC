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
 * @brief Implements the Least Frequently Used Segment Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage of a segment with the behaviour of the LFU
 * Eviction Strategy
 *
 */
#include <nodes/FrequencyNode.h>
#include "LFUCacheSegment.h"
#include <map>
#include <string>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

/*
 * @brief Creates a new LFU CacheSegment
 * @param size The desired Size of this Cache
 * @param category the content category that is stored in this cache segment
 * @return A pointer to this class
 */
LFUCacheSegment::LFUCacheSegment(long long size, std::string category) {
    // TODO Auto-generated constructor stub
    setSize(size);
    setCategory(category);
    auto f = new FrequencyNode(0, nullptr, nullptr, nullptr);
    f->setNext(f);
    f->setPrev(f);
    this->head = f;
}

LFUCacheSegment::~LFUCacheSegment() {
    // TODO Auto-generated destructor stub
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 */
void LFUCacheSegment::resetRates() {
    this->writeOperation = 0;
    this->readOperation = 0;
}

void LFUCacheSegment::periodicEvents() {
}
/**
 * @brief is called upon a cache hit to rearrange the cache
 *
 * @param pkg the video segment which triggered the rearrangement
 *
 * LFU cache rearrangement is a little complicated. On a cache hit we
 * increment the value that signals the frequency for this stored segment.
 * this is done by moving the video id of the segmet to a frequency node with
 * a higher frequency. If that frequency node does not exist we create it and set
 * the pointers accordingly. If the old frequency node where we moved the
 * video id from is now empty,  we delete it and set the pointers accordingly.
 * In the storage, the pointer to the frequency node is updated.
 */
void LFUCacheSegment::rearrangeCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    FrequencyNode* freq = container[keyBuilder]->second;
    FrequencyNode* next_freq = freq->getNext();

    if ((next_freq->getValue() == 0)
            || (next_freq->getValue() != (freq->getValue() + 1))) {
        std::set<std::string>* items = new std::set<std::string>();
        items->insert(keyBuilder);
        FrequencyNode* newNext_freq = new FrequencyNode(freq->getValue() + 1,
                items, freq, next_freq);
        next_freq->setPrev(newNext_freq);
        freq->setNext(newNext_freq);
    } else {
        next_freq->getItems()->insert(keyBuilder);
    }
    freq->getItems()->erase(keyBuilder);
    container[keyBuilder]->second = freq->getNext();

    if (freq->getItems()->size() == 0) {
        freq->getPrev()->setNext(freq->getNext());
        freq->getNext()->setPrev(freq->getPrev());
        //frequencyList.erase(freq);
        delete freq;
    }
}
/*
 * @brief Assignes a specific category to the cache
 * @param category the category of the elements that should be stored in this cache segemnt
 */
void LFUCacheSegment::setCategory(std::string category) {
    this->category = category;
}
/*
 * @brief get the category of the cache segment
 * returns the category assigned to this cache segment
 *
 * @return the cateogry assigned to ths cache segment
 */
std::string LFUCacheSegment::getCategory() {
    return this->category;
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 * @todo Check if this should really be public
 */
void LFUCacheSegment::setSize(long long size) {
    this->maxCacheSize = size;
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long LFUCacheSegment::getSize() {
    return this->cacheSize;
}
/**
 * @brief Get the maximum size of the cache
 * @return Returns an integer Value describing the maximum size of the cache
 */
long long LFUCacheSegment::getMaxSize(){
    return this->maxCacheSize;
}
/**
 * @brief deletes all Objects in the Cache
 */
void LFUCacheSegment::clearCache() {
    for (auto i : container) {
        std::string toDelete = i.first;
        delete (container[toDelete]->first);
        delete (container[toDelete]);
    }

    FrequencyNode* freq = head->getNext();
    while (freq->getValue() != 0) {
        freq = freq->getNext();
        delete freq->getPrev();
    }

    delete head;
}

/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool LFUCacheSegment::contains(SegmentRequest *rqst) {
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
 * When a video segment is deleted, we check if the corresponding frequency node
 * is now empty. if so, we update the pointers and delete the frequency node in
 * order to save storage.
 * @param id the video id of the video segment that has to be deleted
 */
void LFUCacheSegment::deleteSegment(std::string id) {
    if (container.find(id) != container.end()) {
        FrequencyNode* freq = container[id]->second;
        freq->getItems()->erase(id);
        if (freq->getItems()->size() == 0) {
            freq->getPrev()->setNext(freq->getNext());
            freq->getNext()->setPrev(freq->getPrev());
            delete (freq);

        }
        int freedSize = container[id]->first->getSize();
        cacheSize = cacheSize - freedSize;
        delete (container[id]->first);
        delete (container[id]);
        container.erase(id);
        writeOperation++;
    }
}
/**
 * @brief Inserts a video segment into the Cache
 *
 * We first check, if there is enough space in the cache to insert the video segment.
 * If not, the least frequently used videopackages are deleted until enough space is freed up.
 * When a video segment is inserted into the cache, we check if a FrequencyNode with
 * the frequency 1 exists. If not, a new one is created. We then insert the id into
 * the frequency node with frequency 1, generate a pair of the segment and a pointer
 * to the FrequencyNode and store it with the videoId as the key in the storage.
 *
 * @param *pkg A VideoSegment
 * @return a list with all the ids of videos that were deleted
 */
std::list<std::string>* LFUCacheSegment::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        toDelete = getLeastFrequent();
        deletePackage(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    FrequencyNode* freq = head->getNext();
    if (freq->getValue() != 1) {
        std::set<std::string>* items = new std::set<std::string>();
        items->insert(keyBuilder);
        auto p = new std::pair<VideoSegment*, FrequencyNode*>(pkg,
                new FrequencyNode(1, items, head, freq));
        container[keyBuilder] = p;
        freq->setPrev(p->second);
        head->setNext(p->second);

    } else {
        freq->getItems()->insert(keyBuilder);
        auto p = new std::pair<VideoSegment*, FrequencyNode*>(pkg, freq);
        container[keyBuilder] = p;
    }
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
std::list<std::string>* LFUCacheSegment::reduce(int size) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - size) {
        toDelete = getLeastFrequent();
        deletePackage(toDelete);
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
VideoSegment *LFUCacheSegment::retrieveSegment(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
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
void LFUCacheSegment::expand(int i) {
    maxCacheSize = maxCacheSize + i;
}
/**
 * @brief returns the id of the least frequently used video segment
 *
 * pretty self explanatory
 *
 * @return the id of the least frequently used video segment as a string
 */
std::string LFUCacheSegment::getLeastFrequent() {
    return *head->getNext()->getItems()->begin();
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int LFUCacheSegment::getReadOperations() {
    return this->readOperation;
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int LFUCacheSegment::getWriteOperations() {
    return this->writeOperation;
}
