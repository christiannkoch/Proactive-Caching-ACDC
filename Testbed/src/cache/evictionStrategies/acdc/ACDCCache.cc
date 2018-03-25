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
 * @brief Implements the Adaptive Content-Aware Designed Cache Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the ACDC
 * Eviction Strategy
 *
 */
#include "ACDCCache.h"

#include <ghostLists/GhostList.h>
#include "ProbationaryCacheFactory.h"
#include "CacheSegmentFactory.h"
#include "LifeTimeGhostList.h"
#include <string>
#include "SSASFactory.h"
#include "BasicSSAS.h"

ACARCCache::ACARCCache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        const std::string& storageAlterationStrategy) {
    this->parameters = parameters;
    setSize(size);
    expandStrat = storageAlterationStrategy;
    this->storageAlterations = storageAlterations;
    if(storageAlterations->size() > 0){
        expanded = false;
    }
    else
        expanded = true;
    this->minSegSize = std::stof(
            this->parameters->at(this->parameters->size() - 1));
    try {
        this->probationaryCache =
                (ARCProbationaryCache*) ProbationaryCacheFactory::createProbationaryCache(
                        subCacheSize, "ARC");
    } catch (char const* msg) {
        std::cout << msg;
    }
    this->adaptiveStrategy = parameters->at(0);
    setupCacheSegments();
    if (this->adaptiveStrategy == "timeLimitedGhostList")
        this->probationaryGhostList = new LifeTimeGhostList();
    else {
        this->probationaryGhostList = new LifeTimeGhostList();
    }
    probationaryVector.setName("ProbationaryCacheSize");
    ghostListSize.setName("ProbationaryGhostListSize");
    adaptionStrategy = SSASFactory::createAdaptionStrategy(probationaryCache,
            probationaryGhostList, &cacheSegmentVector, &ghostListVector,
            cacheSize, subCacheSize, minSegSize, adaptiveStrategy);
}

void ACARCCache::resetRates() {
    probationaryVector.record(probationaryCache->getSize());
    ghostListSize.record(probationaryGhostList->getSize());
    for (unsigned int i = 0; i < cacheSegmentVector.size(); i++) {
        cacheSegemntSizes[i]->record(cacheSegmentVector[i]->getSize());
    }
    for (unsigned int i = 0; i < ghostListVector.size(); i++) {
        ghostListSizes[i]->record(ghostListVector[i]->getSize());
    }
    for (auto i : cacheSegmentVector)
        i->resetRates();
    probationaryCache->resetRates();
    probationaryGhostList->periodicEvents();
    for (auto i : ghostListVector)
        i->periodicEvents();
    if (!expanded and (omnetpp::simTime().dbl() > storageAlterations->begin()->first)) {
        alterCacheSize(storageAlterations->begin()->second);
        storageAlterations->erase(storageAlterations->begin());
        if(storageAlterations->size() == 0){
            expanded = true;
        }
    }
}

void ACARCCache::deletePackage(std::string id) {
    probationaryCache->deletePackage(id);
    probationaryGhostList->deleteEntry(id);
    for (auto i : cacheSegmentVector) {
        i->deletePackage(id);
    }
    for (auto i : ghostListVector) {
        i->deleteEntry(id);
    }
}

ACARCCache::~ACARCCache() {
    delete probationaryCache;
    delete probationaryGhostList;
    for (auto i : cacheSegmentVector)
        delete i;
    for (auto i : ghostListVector)
        delete i;
    for (auto i : cacheSegemntSizes)
        delete i;
    for (auto i : ghostListSizes)
        delete i;
    delete adaptionStrategy;
    delete storageAlterations;
}

void ACARCCache::periodicEvents() {
    probationaryCache->periodicEvents();
    probationaryGhostList->periodicEvents();
    for (auto i : cacheSegmentVector)
        i->periodicEvents();
    for (auto i : ghostListVector)
        i->periodicEvents();

}

void ACARCCache::insertIntoCache(VideoSegment* pkg) {
    std::list<std::string>* IDtoInsert = probationaryCache->insertIntoCache(
            pkg);
    std::string idBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    probationaryGhostList->deleteEntry(idBuilder);
    if (!IDtoInsert->empty())
        probationaryGhostList->insert(IDtoInsert);
    else
        delete IDtoInsert;
}

bool ACARCCache::contains(SegmentRequest* rqst) {
    std::string idBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    int toSearch = findCacheForCategory(rqst);

    if (probationaryCache->contains(rqst))
        return true;
    else if (cacheSegmentVector.at(toSearch)->contains(rqst))
        return true;
    else {
        if (probationaryGhostList->contains(idBuilder)) {
            int toReduce = adaptionStrategy->getIdToReduce(0) - 1;
            if (cacheSegmentVector.at(toReduce)->getSize()
                    > (subCacheSize / minSegSize)) {
                probationaryCache->expand(rqst->getSize());
                std::list<std::string>* toInsert = cacheSegmentVector.at(
                        toReduce)->reduce(rqst->getSize());
                if (!toInsert->empty())
                    ghostListVector.at(toReduce)->insert(toInsert);
                else
                    delete toInsert;
            }
        }
        return false;
    }
}

VideoSegment* ACARCCache::retrievePackage(SegmentRequest* rqst) {
    if (probationaryCache->contains(rqst)) {
        VideoSegment* dummy = probationaryCache->retrievePackage(rqst);
        rearrangeCache(dummy);
        return cacheSegmentVector.at(findCacheForCategory(rqst))->retrievePackage(
                rqst);
    } else {
        return cacheSegmentVector.at(findCacheForCategory(rqst))->retrievePackage(
                rqst);
    }
}

long long ACARCCache::getSize() {
    return this->cacheSize;
}

void ACARCCache::clearCache() {
    for (auto i : cacheSegmentVector)
        i->clearCache();
    probationaryCache->clearCache();
}

int ACARCCache::getWriteOperations() {
    int writeOperations = this->probationaryCache->getWriteOperations();
    for (auto i : cacheSegmentVector)
        writeOperations = writeOperations + i->getWriteOperations();
    return writeOperations;
}

int ACARCCache::getReadOperations() {
    int readOpreations = this->probationaryCache->getReadOperations();
    for (auto i : cacheSegmentVector)
        readOpreations = readOpreations + i->getReadOperations();
    return readOpreations;
}
//hier muss -2 gerechnet werden, da der erste Parameter immer die AdaptionStrategy angibt und der letzte die minSegSize
//am ende noch +1 rechnen wegen des ProbationaryCache
void ACARCCache::setSize(long long size) {
    this->cacheSize = size;
    subCacheSize = (cacheSize / (((this->parameters->size() - 2) / 2) + 1));
}

void ACARCCache::rearrangeCache(VideoSegment *pkg) {
    std::string idBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    probationaryCache->drop(pkg);
    int toExpand = findGhostListForCategory(pkg);
    if (ghostListVector.at(toExpand)->contains(idBuilder)) {
        int toReduce = adaptionStrategy->getIdToReduce(toExpand + 1);
        if ((toReduce == 0)
                && (probationaryCache->getSize() > (subCacheSize / minSegSize))) {
            std::list<std::string>* toInsert = probationaryCache->reduce(
                    pkg->getSize());
            cacheSegmentVector.at(toExpand)->expand(pkg->getSize());
            if (!toInsert->empty())
                probationaryGhostList->insert(toInsert);
            else
                delete toInsert;
        } else if ((toReduce > 0)
                && cacheSegmentVector.at(toReduce - 1)->getSize()
                        > (subCacheSize * (1 / minSegSize))) {
            std::list<std::string>* toInsert = cacheSegmentVector.at(
                    toReduce - 1)->reduce(pkg->getSize());
            cacheSegmentVector.at(toExpand)->expand(pkg->getSize());
            if (!toInsert->empty())
                ghostListVector.at(toReduce - 1)->insert(toInsert);
            else
                delete toInsert;
        }
    }
    ghostListVector.at(toExpand)->deleteEntry(idBuilder);
    std::list<std::string>* IDtoInsert =
            cacheSegmentVector.at(toExpand)->insertIntoCache(pkg);
    if (!IDtoInsert->empty())
        ghostListVector.at(toExpand)->insert(IDtoInsert);
    else
        delete IDtoInsert;
}

void ACARCCache::setupCacheSegments() {
    for (unsigned int i = 1; i < parameters->size() - 1; i = i + 2) {
        std::string typeParam = parameters->at(i);
        std::string categoryParam = parameters->at(i + 1);
        try {
            cacheSegmentVector.push_back(
                    CacheSegmentFactory::createCacheSegment(typeParam,
                            categoryParam, subCacheSize));
        } catch (char const* msg) {
            std::cout << msg;
        }
        BasicGhostList* ghost;
        if (this->adaptiveStrategy == "timeLimitedGhostList")
            ghost = new LifeTimeGhostList();
        else {
            ghost = new GhostList();
        }
        ghostListVector.push_back(ghost);

        omnetpp::cOutVector* newOutVec = new omnetpp::cOutVector();
        std::string NameBuilder = categoryParam + " Segment Size";
        newOutVec->setName(NameBuilder.c_str());
        cacheSegemntSizes.push_back(newOutVec);

        omnetpp::cOutVector* newGhostOutVec = new omnetpp::cOutVector();
        NameBuilder = categoryParam + " ghost List Size";
        newGhostOutVec->setName(NameBuilder.c_str());
        ghostListSizes.push_back(newGhostOutVec);
    }
}

void ACARCCache::alterCacheSize(double newCacheSize) {
    if (expandStrat == "equal") {
        double toExpand = newCacheSize / (cacheSegmentVector.size() + 1);
        subCacheSize = toExpand;
        cacheSize = newCacheSize;
        probationaryCache->setSize(toExpand);
        for (unsigned int i = 0; i < cacheSegmentVector.size(); i++) {
            cacheSegmentVector[i]->setSize(toExpand);
        }
    }
    if (expandStrat == "partial") {
        subCacheSize = newCacheSize / (cacheSegmentVector.size() + 1);
        double share = (double) probationaryCache->getMaxSize()
                / (double) cacheSize;
        probationaryCache->setSize(newCacheSize * share);
        for (unsigned int i = 0; i < cacheSegmentVector.size(); i++) {
            share = (double) cacheSegmentVector[i]->getMaxSize()
                    / (double) cacheSize;
            cacheSegmentVector[i]->setSize(newCacheSize * share);
        }
        cacheSize = newCacheSize;
    }
}

/*
 * Gibt einen integer zwischen 0 und (N-1) zurück, wobei 0 bedeutet, dass es das MISC-Segment ist, es also kein explizites Cache-Segment
 * für diesen Cache gibt. Alles andere gibt eben den Indize wieder.
 */
int ACARCCache::findCacheForCategory(SegmentRequest* rqst) {
    for (unsigned int i = 0; i < cacheSegmentVector.size(); i++)
        if (cacheSegmentVector.at(i)->getCategory().compare(rqst->getCategory())
                == 0)
            return i;
    return 0;
}
/*
 * Schaut nach, ob eine PacketID in einer der Ghostlists drinnen steht. Zurückgegeben wird ein Wert zwischen 0 und (N-1), der für die
 * Ghostlist steht, die zur Zugehörigen Kategorie zählt. 0 als Rückgabe bedeutet MISC.
 */
int ACARCCache::findGhostListForCategory(VideoSegment* pkg) {
    int cacheSegment = 0;
    for (unsigned int i = 0; i < cacheSegmentVector.size(); i++)
        if (cacheSegmentVector.at(i)->getCategory().compare(pkg->getCategory())
                == 0)
            cacheSegment = i;
    return cacheSegment;
}
