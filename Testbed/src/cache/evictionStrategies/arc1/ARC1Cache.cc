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
/* @file ARC1Cache.cc
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
#include "ARC1Cache.h"
#include <omnetpp.h>
#include <map>
#include <string>
#include <vector>
#include <sstream>

ARC1Cache::ARC1Cache(std::vector<std::string>* parameters, long long size,
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
        this->t2 = (LFUCacheSegment*) CacheSegmentFactory::createCacheSegment(
                "LFU", "Misc", subCacheSize);
    } catch (char const* msg) {
        std::cout << msg;
    }

}

void ARC1Cache::resetRates() {
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

void ARC1Cache::deleteSegment(std::string id) {
    t1->deleteSegment(id);
    b1->deleteEntry(id);
    t2->deleteSegment(id);
    b2->deleteEntry(id);
}

ARC1Cache::~ARC1Cache() {
    delete t1;
    delete t2;
    delete b1;
    delete b2;
    delete storageAlterations;
}

void ARC1Cache::periodicEvents() {
    t1->periodicEvents();
    t2->periodicEvents();
    b1->periodicEvents();
    b2->periodicEvents();
}

void ARC1Cache::insertIntoCache(VideoSegment *pkg) {
    std::list<std::string>* IDtoInsert = t1->insertIntoCache(pkg);
    std::string idBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    b1->deleteEntry(idBuilder);
    if (!IDtoInsert->empty())
        b1->insert(IDtoInsert);
}

bool ARC1Cache::contains(SegmentRequest *rqst) {
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
                }
            }
        }
        return false;
    }

}

VideoSegment *ARC1Cache::retrieveSegment(SegmentRequest *rqst) {
    if (t1->contains(rqst)) {
        VideoSegment* dummy = t1->retrieveSegment(rqst);
        rearrangeCache(dummy);
        return t2->retrieveSegment(rqst);
    } else {
        return t2->retrieveSegment(rqst);
    }
}

void ARC1Cache::setSize(long long size) {
    this->cacheSize = size;
    subCacheSize = (size / 2);
}

long long ARC1Cache::getSize() {
    return this->cacheSize;
}

void ARC1Cache::clearCache() {
    t1->clearCache();
    t2->clearCache();
}

void ARC1Cache::rearrangeCache(VideoSegment *pkg) {
    t1->drop(pkg);
    std::string idBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    if (b2->contains(idBuilder)) {
        if (t1->getSize() > 1) {
            t2->expand(1);
            std::list<std::string>* toInsert = t1->reduce(1);
            if (!toInsert->empty()) {
                b1->insert(toInsert);
            }

        }
    }
    b2->deleteEntry(idBuilder);
    std::list<std::string>* IDtoInsert = t2->insertIntoCache(pkg);
    if (!IDtoInsert->empty()) {
        b2->insert(IDtoInsert);
    }
}

void ARC1Cache::alterCacheSize(double newCacheSize) {
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

int ARC1Cache::getWriteOperations() {
    return t1->getWriteOperations() + t2->getWriteOperations();
}

int ARC1Cache::getReadOperations() {
    return t1->getReadOperations() + t2->getReadOperations();
}

std::string ARC1Cache::getCountsOfElements() {
    std::stringstream buf;
    buf << "Probationary: " << t1->getCountsOfElements()
            << "; Protected: " << t2->getCountsOfElements() << "; ";
    return buf.str();
}

