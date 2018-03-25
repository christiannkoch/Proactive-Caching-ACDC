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
    }
    else{
        expanded = true;
    }
}

RANDCache::~RANDCache() {
    delete storageAlterations;
}

void RANDCache::periodicEvents() {

}

void RANDCache::deletePackage(std::string id) {
    int freedSize = container[id]->getSize();
    delete container[id];
    container.erase(id);
    keyList.erase(std::remove(keyList.begin(), keyList.end(), id),
            keyList.end());
    cacheSize = cacheSize - freedSize;
    writeOperation++;
}

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
bool RANDCache::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}

void RANDCache::alterCacheSize(double newCacheSize) {
    maxCacheSize = newCacheSize;
}

VideoSegment* RANDCache::retrievePackage(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    return container[keyBuilder]->dup();
}
long long RANDCache::getSize() {
    return this->cacheSize;
}
void RANDCache::clearCache() {
    for (auto i : container) {
        delete i.second;
    }
}
int RANDCache::getWriteOperations() {
    return this->writeOperation;
}
int RANDCache::getReadOperations() {
    return this->readOperation;
}
void RANDCache::setSize(long long size) {
    cacheSize = size;
}
void RANDCache::rearrangeCache(VideoSegment *pkg) {

}
