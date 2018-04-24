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
/* @file ARC2Cache.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements a version of the Adaptive Replacement Cache Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the ARC
 * Eviction Strategy
 *
 */
#include <ghostLists/LimitedGhostList.h>
#include <map>
#include <string>
#include <vector>
#include "ARC2Cache.h"
#include "LRUCacheSegment.h"
#include "ARCProbationaryCache.h"
#include "ProbationaryCacheFactory.h"
#include "CacheSegmentFactory.h"
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "ReverseProxy.h"
/*
 * @brief Creates a new ARC Cache for caching functionalities
 * @param size The desired Size of this Cache
 * @param storageAlterations a vector of pairs of double value representing storage alterations
 * @param storageAlterationStrategy the strategy used when altering the size of the storage
 * @return A pointer to this class
 */
ARC2Cache::ARC2Cache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy) {
    this->parameters = parameters;
    setSize(size);
    expandStrat = storageAlterationStrategy;
    this->storageAlterations = storageAlterations;
    if (storageAlterations->size() > 0) {
        expanded = false;
    } else
        expanded = true;
    try {
        this->t1 =
                (ARCProbationaryCache*) ProbationaryCacheFactory::createProbationaryCache(
                        subCacheSize, "ARC");
    } catch (char const* msg) {
        std::cout << msg;
    }
    try {
        this->t2 = (LRUCacheSegment*) CacheSegmentFactory::createCacheSegment(
                "LRU", "Misc", subCacheSize);
    } catch (char const* msg) {
        std::cout << msg;
    }
    this->b1 = new LimitedGhostList(subCacheSize / 75);
    this->b2 = new LimitedGhostList(subCacheSize / 75);
}
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 * Also triggers the storage alteration if any exist
 */
void ARC2Cache::resetRates() {
    t1->resetRates();
    t2->resetRates();
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
 * This is used when the admission strategy is MCD
 * @param id the video id of the video segment that has to be deleted
 */
void ARC2Cache::deleteSegment(std::string id) {
    t1->deleteSegment(id);
    b1->deleteEntry(id);
    t2->deleteSegment(id);
    b2->deleteEntry(id);
}

ARC2Cache::~ARC2Cache() {
    delete t1;
    delete t2;
    delete b1;
    delete b2;
    delete storageAlterations;
}
/**
 * @brief executes periodic events
 *
 * periodic events can regularily check on something
 */
void ARC2Cache::periodicEvents() {
    t1->periodicEvents();
    t2->periodicEvents();
    b1->periodicEvents();
    b2->periodicEvents();
}
/**
 * @brief Inserts a VideoSegment into the Cache
 *
 * The
 * @param *pkg A VideoSegment
 */
void ARC2Cache::insertIntoCache(VideoSegment *pkg) {
    std::list<std::string>* IDtoInsert = t1->insertIntoCache(pkg);
    std::string idBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    b1->deleteEntry(idBuilder);
    if (!IDtoInsert->empty())
        b1->insert(IDtoInsert);
    else {
        delete IDtoInsert;
    }
}
/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool ARC2Cache::contains(SegmentRequest *rqst) {
    std::string idBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (t1->contains(rqst)) {
        return true;
    } else if (t2->contains(rqst)) {
        return true;
    } else {
        if (b1->contains(idBuilder)) {
            if (t2->getSize() > 1) {
                t1->expand(1);
                std::list<std::string>* toInsert = t2->reduce(1);
                if (!toInsert->empty()) {
                    b2->insert(toInsert);
                } else {
                    delete toInsert;
                }
            }
        }
        return false;
    }

}
/**
 * @brief Retrieves the video segment from the Cache. This should only be executed, if contains returns true.
 * @param rqst A segment request
 * @return The video segment that fullfills the segment request
 *
 */
VideoSegment *ARC2Cache::retrieveSegment(SegmentRequest *rqst) {
    if (t1->contains(rqst)) {
        VideoSegment* dummy = t1->retrieveSegment(rqst);
        rearrangeCache(dummy);
        return t2->retrieveSegment(rqst);
    } else {
        return t2->retrieveSegment(rqst);
    }
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 */
void ARC2Cache::setSize(long long size) {
    this->cacheSize = size;
    subCacheSize = (size / 2);
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long ARC2Cache::getSize() {
    return this->cacheSize;
}
/**
 * @brief deletes all Objects in the Cache
 */
void ARC2Cache::clearCache() {
    t1->clearCache();
    t2->clearCache();
}
/**
 * @brief is called upon a cache hit to rearrange the cache
 *
 * When a cache hit occurs, the cache is rearranged. If it was in the probationary part of the
 * cache, the segment is moved from the probationary part to the LRU segment.
 * If segments from the lru part have to be deleted, their video id's are stored
 * in the lru ghostlist.
 */
void ARC2Cache::rearrangeCache(VideoSegment *pkg) {
    t1->drop(pkg);
    std::string idBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    if (b2->contains(idBuilder)) {
        if (t1->getSize() > pkg->getSize()) {
            t2->expand(pkg->getSize());
            std::list<std::string>* toInsert = t1->reduce(pkg->getSize());
            if (!toInsert->empty()) {
                b1->insert(toInsert);
            } else {
                delete toInsert;
            }

        }
    }
    b2->deleteEntry(idBuilder);
    std::list<std::string>* IDtoInsert = t2->insertIntoCache(pkg);
    if (!IDtoInsert->empty()) {
        b2->insert(IDtoInsert);
    } else {
        delete IDtoInsert;
    }
}
/**
 * @brief alters the cache size
 *
 * a function that alters the maximum size of the cache to a given double value.
 *
 * @param newCacheSize a double value representing the new size of the cache
 */
void ARC2Cache::alterCacheSize(double newCacheSize) {
    if (expandStrat == "equal") {
        double toExpand = (newCacheSize / 2);
        subCacheSize = toExpand;
        t1->setSize(toExpand);
        t2->setSize(toExpand);
        cacheSize = newCacheSize;
    }
    if (expandStrat == "partial") {
        subCacheSize = (newCacheSize / 2);
        double share = (double) t1->getMaxSize() / (double) cacheSize;
        t1->setSize(newCacheSize * share);
        share = (double) t2->getMaxSize() / (double) cacheSize;
        t2->setSize(newCacheSize * share);
        cacheSize = newCacheSize;
    }
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int ARC2Cache::getWriteOperations() {
    return t1->getWriteOperations() + t2->getWriteOperations();
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int ARC2Cache::getReadOperations() {
    return t1->getReadOperations() + t2->getReadOperations();
}

