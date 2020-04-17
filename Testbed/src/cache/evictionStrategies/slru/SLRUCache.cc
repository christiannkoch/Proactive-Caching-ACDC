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
/* @file SLRUCache.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the Segmented Least Recently Used Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the SLRU
 * Eviction Strategy
 *
 */
#include "SLRUCache.h"
#include "ProbationaryCacheFactory.h"
#include "CacheSegmentFactory.h"
#include <sstream>
/*
 * @brief Creates a new SLRUCache for caching functionalities
 * @param parameters the parameters for this eviction strategy
 * @param size The desired Size of this Cache
 * @param storageAlterations a vector of pairs of double value representing storage alterations
 * @param storageAlterationStrategy the strategy used when altering the size of the storage
 * @return A pointer to this class
 */
SLRUCache::SLRUCache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy) {
    this->parameters = parameters;
    expandStrat = storageAlterationStrategy;
    expandStrat = storageAlterationStrategy;
    this->storageAlterations = storageAlterations;
    setSize(size);
    if (storageAlterations->size() > 0) {
        expanded = false;
    } else
        expanded = true;
    try {
        probationaryCache =
                (SLRUProbationaryCache*) ProbationaryCacheFactory::createProbationaryCache(
                        subCacheSize, "SLRU");
    } catch (char const* msg) {
        std::cout << msg;
    }

    try {
        protectedCache =
                (LRUCacheSegment*) CacheSegmentFactory::createCacheSegment(
                        "LRU", "MISC", subCacheSize);
    } catch (char const* msg) {
        std::cout << msg;
    }
}
/**
 * @brief deletes video segment from the storage of the cache
 *
 * @param id the video id of the video segment that has to be deleted
 */
void SLRUCache::deleteSegment(std::string id) {
    probationaryCache->deleteSegment(id);
    protectedCache->deleteSegment(id);
}
/**
 * @brief alters the cache size
 *
 * a function that alters the maximum size of the cache to a given double value
 *
 * @param newCacheSize a double value representing the new size of the cache
 */
void SLRUCache::alterCacheSize(double newCacheSize) {
    if (expandStrat == "equal") {
        double toExpand = (newCacheSize / 2);
        subCacheSize = toExpand;
        probationaryCache->setSize(toExpand);
        protectedCache->setSize(toExpand);
        cacheSize = newCacheSize;
    }
    if (expandStrat == "partial") {
        subCacheSize = (newCacheSize / 2);
        double share = (double) probationaryCache->getMaxSize()
                / (double) cacheSize;
        probationaryCache->setSize(newCacheSize * share);
        share = (double) protectedCache->getMaxSize() / (double) cacheSize;
        protectedCache->setSize(newCacheSize * share);
        cacheSize = newCacheSize;
    }
}

SLRUCache::~SLRUCache() {
    delete probationaryCache;
    delete protectedCache;
    delete storageAlterations;
}
/**
 * @brief executes periodic events
 *
 * periodic events can regularily check on something
 */
void SLRUCache::periodicEvents() {
}
/**
 * @brief Inserts a video segment into the Cache
 *
 * A segment is inserted into the cache following the SLRU algorithm
 * @param *pkg A VideoSegment
 */
void SLRUCache::insertIntoCache(VideoSegment *pkg) {
    probationaryCache->insertIntoCache(pkg);
}
/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool SLRUCache::contains(SegmentRequest *rqst) {
    if (probationaryCache->contains(rqst))
        return true;
    else if (protectedCache->contains(rqst))
        return true;
    else
        return false;
}
/**
 * @brief Retrieves the video segment from the Cache. This should only be executed, if contains returns true.
 * @param rqst A segment request
 *
 * The video segment is returned and we call rearrangeCache.
 * @return The video segment that fullfills the segment request
 *
 */
VideoSegment* SLRUCache::retrieveSegment(SegmentRequest *rqst) {
    if (probationaryCache->contains(rqst)) {
        VideoSegment* dummy = probationaryCache->retrieveSegment(rqst);
        probationaryCache->drop(dummy);
        VideoSegment* protectedDummy =
                protectedCache->getElementWithSecondChance();
        std::list<std::string>* toInsert = protectedCache->insertIntoCache(
                dummy);
        if (!toInsert->empty())
            probationaryCache->insertIntoCacheAtEnd(protectedDummy);
        else
            delete protectedDummy;
        delete toInsert;
        return dummy->dup();
    } else if (protectedCache->contains(rqst)) {
        return protectedCache->retrieveSegment(rqst);
    } else
        return nullptr;
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 */
void SLRUCache::setSize(long long size) {
    subCacheSize = (size / 2);
    this->cacheSize = size;
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long SLRUCache::getSize() {
    return this->cacheSize;
}
/**
 * @brief deletes all Objects in the Cache
 */
void SLRUCache::clearCache() {
    probationaryCache->clearCache();
    protectedCache->clearCache();

}
/*
 * @brief rearranges the cache on hit in the LFUDA manner
 *
 * @param pkg the video segment that triggered the rearrangement
 * Not used here
 */
void SLRUCache::rearrangeCache(VideoSegment *pkg) {

}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 * Also triggers the storage alteration if any exist
 */
void SLRUCache::resetRates() {
    probationaryCache->resetRates();
    protectedCache->resetRates();
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
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int SLRUCache::getReadOperations() {
    return probationaryCache->getReadOperations()
            + protectedCache->getReadOperations();
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int SLRUCache::getWriteOperations() {
    return probationaryCache->getWriteOperations()
            + protectedCache->getWriteOperations();
}

std::string SLRUCache::getCountsOfElements() {
    std::stringstream buf;
    buf << "Probationary: " << probationaryCache->getCountsOfElements()
            << "; Protected: " << protectedCache->getCountsOfElements() << "; ";
    return buf.str();
}

