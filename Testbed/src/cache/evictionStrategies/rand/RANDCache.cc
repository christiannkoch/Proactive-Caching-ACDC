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
/* @file RANDCache.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the Random Cache Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the RAND
 * Eviction Strategy
 *
 */
#include "RANDCache.h"
#include <algorithm>
/*
 * @brief Creates a new RANDCache for caching functionalities
 * @param parameters the parameters for this eviction strategy
 * @param size The desired Size of this Cache
 * @param storageAlterations a vector of pairs of double value representing storage alterations
 * @param storageAlterationStrategy the strategy used when altering the size of the storage
 * @return A pointer to this class
 */
RANDCache::RANDCache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy) {
    // TODO Auto-generated constructor stub
    this->parameters = parameters;
    setSize(size);
    expandStrat = storageAlterationStrategy;
    this->storageAlterations = storageAlterations;
    if (storageAlterations->size() > 0) {
        expanded = false;
    } else {
        expanded = true;
    }
}

RANDCache::~RANDCache() {
    delete storageAlterations;
}
/**
 * @brief executes periodic events
 *
 * periodic events can regularily check on something
 */
void RANDCache::periodicEvents() {

}
/**
 * @brief deletes video segment from the storage of the cache
 *
 * @param id the video id of the video segment that has to be deleted
 */
void RANDCache::deleteSegment(std::string id) {
    int freedSize = container[id]->getSize();
    delete container[id];
    container.erase(id);
    keyList.erase(std::remove(keyList.begin(), keyList.end(), id),
            keyList.end());
    cacheSize = cacheSize - freedSize;
    writeOperation++;
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 * Also triggers the storage alteration if any exist
 */
void RANDCache::resetRates() {
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
 * @brief Inserts a video segment into the Cache
 *
 * A segment is inserted into the cache following the RAND algorithm
 * @param *pkg A VideoSegment
 */
void RANDCache::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        int random = rand() % container.size();
        std::string toRemove = keyList[random];
        delete container[toRemove];
        container.erase(toRemove);
        std::vector<std::string>::iterator nth = keyList.begin() + random;
        keyList.erase(nth);
    }
    container[keyBuilder] = pkg;
    keyList.push_back(keyBuilder);
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
}
/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool RANDCache::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}
/**
 * @brief alters the cache size
 *
 * a function that alters the maximum size of the cache to a given double value
 *
 * @param newCacheSize a double value representing the new size of the cache
 */
void RANDCache::alterCacheSize(double newCacheSize) {
    maxCacheSize = newCacheSize;
}
/**
 * @brief Retrieves the video segment from the Cache. This should only be executed, if contains returns true.
 * @param rqst A segment request
 *
 * The video segment is returned and we call rearrangeCache.
 * @return The video segment that fullfills the segment request
 *
 */
VideoSegment* RANDCache::retrieveSegment(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    return container[keyBuilder]->dup();
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long RANDCache::getSize() {
    return this->cacheSize;
}
void RANDCache::clearCache() {
    for (auto i : container) {
        delete i.second;
    }
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int RANDCache::getWriteOperations() {
    return this->writeOperation;
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int RANDCache::getReadOperations() {
    return this->readOperation;
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 */
void RANDCache::setSize(long long size) {
    cacheSize = size;
}

/*
 * @brief rearranges the cache on hit in the RAND manner
 *
 * with RAND nothing happenes here
 * @param pkg the video segment that triggered the rearrangement
 */
void RANDCache::rearrangeCache(VideoSegment *pkg) {

}
