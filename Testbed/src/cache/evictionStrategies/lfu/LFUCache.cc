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
/* @file LFUCache.cc
 * @author Johannes Pfannmüller
 * @date
 * @version 1.0
 *
 * @brief Implements the Least Frequently Used Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the LFU
 * Eviction Strategy
 *
 */
#include <nodes/FrequencyNode.h>
#include "LFUCache.h"
#include <map>
#include <string>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
/*
 * @brief Creates a new LFU Cache for caching functionalities
 * @param size The desired Size of this Cache
 * @param storageAlterations a vector of pairs of double value representing storage alterations
 * @param storageAlterationStrategy the strategy used when altering the size of the storage
 * @return A pointer to this class
 */
LFUCache::LFUCache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy) {
    this->parameters = parameters;
    setSize(size);
    auto f = new FrequencyNode(0, nullptr, nullptr, nullptr);
    f->setNext(f);
    f->setPrev(f);
    this->head = f;
    expandStrat = storageAlterationStrategy;
    this->storageAlterations = storageAlterations;
    if (storageAlterations->size() > 0) {
        expanded = false;
    } else
        expanded = true;
}

LFUCache::~LFUCache() {
    delete storageAlterations;
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 * Also triggers the storage alteration if any exist
 */
void LFUCache::resetRates() {
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
 * When a video segment is deleted, we check if the corresponding frequency node
 * is now empty. if so, we update the pointers and delete the frequency node in
 * order to save storage.
 * @param id the video id of the video segment that has to be deleted
 */
void LFUCache::deletePackage(std::string id) {
    int freedSize = container[id]->first->getSize();
    FrequencyNode* freq = container[id]->second;
    freq->getItems()->erase(id);
    if (freq->getItems()->size() == 0) {
        freq->getPrev()->setNext(freq->getNext());
        freq->getNext()->setPrev(freq->getPrev());
        delete (freq);
        writeOperation++;
    }
    delete (container[id]->first);
    delete (container[id]);
    container.erase(id);
    cacheSize = cacheSize - freedSize;
    readOperation++;
    writeOperation++;
}
/**
 * @brief executes periodic events
 *
 * periodic events can regularily check on something
 */
void LFUCache::periodicEvents() {

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
 */
void LFUCache::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        std::string toDelete = getLeastFrequent();
        deletePackage(toDelete);
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
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
}

/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool LFUCache::contains(SegmentRequest *rqst) {
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
VideoSegment *LFUCache::retrievePackage(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
    rearrangeCache(pkg);
    return pkg->dup();
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 * @todo Check if this should really be public
 */
void LFUCache::setSize(long long size) {
    maxCacheSize = size;
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 * @todo Do I need this?
 */
long long LFUCache::getSize() {
    return this->cacheSize;
}

/**
 * @brief deletes all Objects in the Cache
 */
void LFUCache::clearCache() {
    /*for (auto iter : container) {
     delete (iter.second);
     }*/
    //delete head;
    for (auto i : container) {
        delete i.second->first;
        delete i.second;
    }
    FrequencyNode* freq = head->getNext();
    while (freq->getValue() != 0) {
        freq = freq->getNext();
        delete freq->getPrev();
    }

    delete head;
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
void LFUCache::rearrangeCache(VideoSegment *pkg) {
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
        delete freq;
    }
}
/**
 * @brief alters the cache size
 *
 * a function that alters the maximum size of the cache to a given double value
 *
 * @param newCacheSize a double value representing the new size of the cache
 */
void LFUCache::alterCacheSize(double newCacheSize) {
    maxCacheSize = newCacheSize;
}
/**
 * @brief returns the id of the least frequently used video segment
 *
 * pretty self explanatory
 *
 * @return the id of the least frequently used video segment as a string
 */
std::string LFUCache::getLeastFrequent() {
    return *head->getNext()->getItems()->begin();
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int LFUCache::getReadOperations() {
    return this->readOperation;
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int LFUCache::getWriteOperations() {
    return this->writeOperation;
}

