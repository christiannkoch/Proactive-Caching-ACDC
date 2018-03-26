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
 * @brief Implements the Infinity Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the INFINITY
 * Eviction Strategy
 *
 */
#include "InfinityCache.h"
#include <string>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

InfinityCache::InfinityCache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy) {
    // TODO Auto-generated constructor stub
    this->parameters = parameters;
    setSize(size);
    expandStrat = storageAlterationStrategy;
    this->storageAlterations = storageAlterations;
}

InfinityCache::~InfinityCache() {
    // TODO Auto-generated destructor stub
    delete storageAlterations;
}

void InfinityCache::resetRates() {
    this->readOperation = 0;
    this->writeOperation = 0;
}

void InfinityCache::deleteSegment(std::string id) {

}

void InfinityCache::periodicEvents() {
}

void InfinityCache::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    container[keyBuilder] = keyBuilder;
    cacheSize = cacheSize + pkg->getSize();
    delete pkg;
    writeOperation++;
}

bool InfinityCache::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}

VideoSegment *InfinityCache::retrieveSegment(SegmentRequest *rqst) {
    readOperation++;
    char pkgname[20];
    sprintf(pkgname, "Package id %s%s", rqst->getVideoId(),
            std::to_string(rqst->getSegmentId()).c_str());
    VideoSegment *pkg = new VideoSegment(pkgname);
    pkg->setVideoId(rqst->getVideoId());
    pkg->setSegmentId(rqst->getSegmentId());
    pkg->setClientId(rqst->getClientId());
    pkg->setUploader(rqst->getUploader());
    pkg->setCategory(rqst->getCategory());
    pkg->setSeenAbove(true);
    return pkg;
}

void InfinityCache::setSize(long long size) {
    this->maxCacheSize = size;
}

long long InfinityCache::getSize() {
    return this->cacheSize;
}

void InfinityCache::clearCache() {

}

void InfinityCache::alterCacheSize(double newCacheSize) {
}

void InfinityCache::rearrangeCache(VideoSegment *pkg) {
}

int InfinityCache::getWriteOperations() {
    return this->writeOperation;
}

int InfinityCache::getReadOperations() {
    return this->readOperation;
}
