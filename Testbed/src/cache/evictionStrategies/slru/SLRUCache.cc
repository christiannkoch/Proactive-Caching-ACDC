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
/* @file ACDCCache.cc
 * @author Johannes Pfannmüller
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

SLRUCache::SLRUCache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy) {
    // TODO Auto-generated constructor stub
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

void SLRUCache::deletePackage(std::string id) {
    probationaryCache->deletePackage(id);
    protectedCache->deletePackage(id);
}

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
    // TODO Auto-generated destructor stub
    delete probationaryCache;
    delete protectedCache;
    delete storageAlterations;
}

void SLRUCache::periodicEvents() {
    /*probationaryCache->printCache();
     protectedCache->printCache();*/
}

void SLRUCache::insertIntoCache(VideoSegment *pkg) {
    probationaryCache->insertIntoCache(pkg);
}

bool SLRUCache::contains(SegmentRequest *rqst) {
    if (probationaryCache->contains(rqst))
        return true;
    else if (protectedCache->contains(rqst))
        return true;
    else
        return false;
}

VideoSegment* SLRUCache::retrievePackage(SegmentRequest *rqst) {
    if (probationaryCache->contains(rqst)) {
        VideoSegment* dummy = probationaryCache->retrievePackage(rqst);
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
        return protectedCache->retrievePackage(rqst);
    } else
        return nullptr;
}

void SLRUCache::setSize(long long size) {
    subCacheSize = (size / 2);
    this->cacheSize = size;
}

long long SLRUCache::getSize() {
    return this->cacheSize;
}

void SLRUCache::clearCache() {
    probationaryCache->clearCache();
    protectedCache->clearCache();

}

void SLRUCache::rearrangeCache(VideoSegment *pkg) {

}

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

int SLRUCache::getReadOperations() {
    return probationaryCache->getReadOperations()
            + protectedCache->getReadOperations();
}

int SLRUCache::getWriteOperations() {
    return probationaryCache->getWriteOperations()
            + protectedCache->getWriteOperations();
}
