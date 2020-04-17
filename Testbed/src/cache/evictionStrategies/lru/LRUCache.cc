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
/* @file LRUCache.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the Least Recently Used Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the LRU
 * Eviction Strategy
 *
 */
#include "LRUCache.h"
#include <list>
#include <string>
#include <vector>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "PointerAndCounter.h"
#include <sstream>
/*
 * @brief Creates a new LRUCache for caching functionalities
 * @param parameters the parameters for this eviction strategy
 * @param size The desired Size of this Cache
 * @param storageAlterations a vector of pairs of double value representing storage alterations
 * @param storageAlterationStrategy the strategy used when altering the size of the storage
 * @return A pointer to this class
 */
LRUCache::LRUCache(std::vector<std::string>* parameters, long long size,
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

LRUCache::~LRUCache() {
    delete storageAlterations;
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 * Also triggers the storage alteration if any exist
 */
void LRUCache::resetRates() {
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
void LRUCache::deleteSegment(std::string id) {
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
/**
 * @brief executes periodic events
 *
 * periodic events can regularily check on something
 */
void LRUCache::periodicEvents() {
}
/**
 * @brief Inserts a video segment into the Cache
 *
 * A segment is inserted into the cache following the LRU algorithm
 * @param *pkg A VideoSegment
 */
void LRUCache::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize >= maxCacheSize - pkg->getSize()) {
        std::string toDelete = head->getPrev()->getValue();
        deleteSegment(toDelete);
        //std::cout << "Insert while schleife vom Loeschen" <<"\n";
    }
    auto p = new std::pair<PointerAndCounter*, RecencyNode*>(new PointerAndCounter(pkg, 0),
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
bool LRUCache::contains(SegmentRequest *rqst) {
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
VideoSegment *LRUCache::retrieveSegment(SegmentRequest *rqst) {
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
 */
void LRUCache::setSize(long long size) {
    this->maxCacheSize = size;
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long LRUCache::getSize() {
    return this->cacheSize;
}
/**
 * @brief deletes all Objects in the Cache
 */
void LRUCache::clearCache() {
    for (auto i : container) {
        delete i.second->first;
        delete i.second->second;
        delete i.second;
    }
    delete head;
}
/*
 * @brief rearranges the cache on hit in the LRU manner
 *
 * @param pkg the video segment that triggered the rearrangement
 */
void LRUCache::rearrangeCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    RecencyNode* rec = container[keyBuilder]->second;
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
void LRUCache::alterCacheSize(double newCacheSize) {
    maxCacheSize = newCacheSize;
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int LRUCache::getWriteOperations() {
    return this->writeOperation;
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int LRUCache::getReadOperations() {
    return this->readOperation;
}

std::string LRUCache::getCountsOfElements(){
    std::stringstream buf;
    for (auto i : container){
        buf << i.first << ", " << i.second->first->getCount() << "; ";
    }

    return buf.str();
}

