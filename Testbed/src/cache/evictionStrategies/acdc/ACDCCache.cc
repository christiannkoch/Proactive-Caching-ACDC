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
 * @author Johannes Pfannmüller, Christian Koch
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
#include <string>
#include <sstream>
#include <algorithm>


/*
 * @brief returns an instance of the ACDCCache
 * @param parameters the parameters for the ACDCCache
 * @param size the maximum cache size
 * @param storageAlterations the vector with the list of storage alterations
 * @param storageAlterationStrategy the strategy that is used when altering the maximum cache size
 * @return an instance of the ACDCCache
 */
ACDCCache::ACDCCache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        const std::string& storageAlterationStrategy) {
    this->parameters = parameters;
    setSize(size);
    expandStrat = storageAlterationStrategy;
    this->storageAlterations = storageAlterations;
    /*
     * if there are no storage alterations expanded can be set to true
     */
    if(storageAlterations->size() > 0){
        expanded = false;
    }
    else
        expanded = true;
    /*
     * calculates the minSegSize
     */
    this->minSegSize = std::stof(
            this->parameters->at(this->parameters->size() - 1));
    /*
     * creates the probationary cache
     */
    try {
        this->probationaryCache =
                (ARCProbationaryCache*) ProbationaryCacheFactory::createProbationaryCache(
                        subCacheSize, "ARC");
    } catch (char const* msg) {
        std::cout << msg;
    }
    this->adaptiveStrategy = parameters->at(0);
    setupCacheSegments();
    /*
     * creates the ghostlist based on the adaptive strategy
     */
    if (this->adaptiveStrategy == "timeLimitedGhostList")
        this->probationaryGhostList = new LifeTimeGhostList();
    else {
        this->probationaryGhostList = new LifeTimeGhostList();
    }
    probationaryVector.setName("ProbationaryCacheSize");
    ghostListSize.setName("ProbationaryGhostListSize");
    /*
     *creates the adaptionStrategy for the cache segments
     */
    adaptionStrategy = SSASFactory::createAdaptionStrategy(probationaryCache,
            probationaryGhostList, &cacheSegmentVector, &ghostListVector,
            cacheSize, subCacheSize, minSegSize, adaptiveStrategy);
}

/*
 * @brief resets the values used for evaluation and triggers the storage alterations
 */
void ACDCCache::resetRates() {
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

/*
 * @brief deletes a segment identified by its video id
 * @param id the video id of the segment to delete
 */
void ACDCCache::deleteSegment(std::string id) {
    probationaryCache->deleteSegment(id);
    probationaryGhostList->deleteEntry(id);
    for (auto i : cacheSegmentVector) {
        i->deleteSegment(id);
    }
    for (auto i : ghostListVector) {
        i->deleteEntry(id);
    }
}

ACDCCache::~ACDCCache() {
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

/*
 * @brief executes periodic Events
 * used only if a TTL segment exists
 */
void ACDCCache::periodicEvents() {
    probationaryCache->periodicEvents();
    probationaryGhostList->periodicEvents();
    for (auto i : cacheSegmentVector)
        i->periodicEvents();
    for (auto i : ghostListVector)
        i->periodicEvents();

}

/*
 * @brief inserts a video segment into the cache
 * @param pkg the video segment to insert into the cache
 *
 * inserts a video segment into the probationary cache
 */
void ACDCCache::insertIntoCache(VideoSegment* pkg) {
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

/*
 * @brief inidcates if a segment requested is in the cache
 * @param rqst a request we want to get the segment for
 *
 * searches the probationary cache first, then all other cache segments
 *
 * @return true if the request is in the cache, false otherwise
 */
bool ACDCCache::contains(SegmentRequest* rqst) {
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

/*
 * @brief retrieves a video segment from the cache
 * @param rqst the request to which we want to retrieve the segment to
 * @return a video segment
 */
VideoSegment* ACDCCache::retrieveSegment(SegmentRequest* rqst) {
    if (probationaryCache->contains(rqst)) {
        VideoSegment* dummy = probationaryCache->retrieveSegment(rqst);
        rearrangeCache(dummy);
        return cacheSegmentVector.at(findCacheForCategory(rqst))->retrieveSegment(
                rqst);
    } else {
        return cacheSegmentVector.at(findCacheForCategory(rqst))->retrieveSegment(
                rqst);
    }
}

/*
 * @brief returns the maximum size of the cache
 * @return the maximum size of the cache
 */
long long ACDCCache::getSize() {
    return this->cacheSize;
}

/*
 * @brief deletes all segments from the cache
 */
void ACDCCache::clearCache() {
    for (auto i : cacheSegmentVector)
        i->clearCache();
    probationaryCache->clearCache();
}

/*
 * @brief returns the amount of write operations performed in a given timeframe
 *
 * @return the amount of write operations
 */
int ACDCCache::getWriteOperations() {
    int writeOperations = this->probationaryCache->getWriteOperations();
    for (auto i : cacheSegmentVector)
        writeOperations = writeOperations + i->getWriteOperations();
    return writeOperations;
}

/*
 * @brief returns the amount of read operations performed in a given timeframe
 *
 * @return the amount of read operations
 */
int ACDCCache::getReadOperations() {
    int readOpreations = this->probationaryCache->getReadOperations();
    for (auto i : cacheSegmentVector)
        readOpreations = readOpreations + i->getReadOperations();
    return readOpreations;
}
/*
 * @brief sets the size of the cache
 * sets the cache size and calculates the subcache size
 * @param size the cache size
 */
void ACDCCache::setSize(long long size) {
    /*
     * we substract 2 because the first parameter is always the adaption strategy and the last parameter the minSegSize
     * at the end we have to add one because of the probationary cache
     */
    this->cacheSize = size;
    subCacheSize = (cacheSize / (((this->parameters->size() - 2) / 2) + 1));
}

/*
 * @brief rearranges the cache
 * @param pkg the video segment wich caused the rearrangement
 * if a hit occured in the probationary cache, the video segment is moved to the
 * corresponding category cache
 * if a hit occured in a catetory cache, the rearrangement of this category cache is
 * triggered
 */
void ACDCCache::rearrangeCache(VideoSegment *pkg) {
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

/*
 * @brief creates the cache segments
 * brief explains this
 */
void ACDCCache::setupCacheSegments() {
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

/*
 * @brief alters the cache size
 * @param newCacheSize the new maximum cache size
 */
void ACDCCache::alterCacheSize(double newCacheSize) {
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
 * @brief returns a value representing a cache corresponding to the category of the request
 * @param rqst the request we want the cache segment for
 *
 * returns an integer betwee 0 and NumberOfCacheSegments-1, 0 means the probationary cache
 *
 * @return the id of the cache segment
 */
int ACDCCache::findCacheForCategory(SegmentRequest* rqst) {
    for (unsigned int i = 0; i < cacheSegmentVector.size(); i++)
        if (cacheSegmentVector.at(i)->getCategory().compare(rqst->getCategory())
                == 0)
            return i;
    return 0;
}

/*
 * @brief returns the ghostlist corresponding to a category
 * @param pkg the segment from which category we want the ghostlist for
 * NOTE: 0 means MISC segment
 * @return the id of the ghostlist corresponding to a category cache
 */
int ACDCCache::findGhostListForCategory(VideoSegment* pkg) {
    int cacheSegment = 0;
    for (unsigned int i = 0; i < cacheSegmentVector.size(); i++)
        if (cacheSegmentVector.at(i)->getCategory().compare(pkg->getCategory())
                == 0)
            cacheSegment = i;
    return cacheSegment;
}

std::string ACDCCache::getCountsOfElements() {
    std::stringstream buf;
    buf << "Probationary: " << probationaryCache->getCountsOfElements() << "; ";
    for (auto i : cacheSegmentVector) {
        buf << i->getCategory() << ": " << i->getCountsOfElements() << "; ";
    }

    return buf.str();
}


