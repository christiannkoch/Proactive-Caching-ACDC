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

#include "RANDCacheSegment.h"
#include <algorithm>

RANDCacheSegment::RANDCacheSegment(long long size, std::string category) {
    // TODO Auto-generated constructor stub
    setSize(size);
    setCategory(category);
}

RANDCacheSegment::~RANDCacheSegment() {
    // TODO Auto-generated destructor stub
}

void RANDCacheSegment::setSize(long long size) {
    this->maxCacheSize = size;
}
long long RANDCacheSegment::getSize() {
    return this->cacheSize;
}

long long RANDCacheSegment::getMaxSize(){
    return this->maxCacheSize;
}

void RANDCacheSegment::resetRates() {
    this->writeOperation = 0;
    this->readOperation = 0;
}

void RANDCacheSegment::periodicEvents() {

}
void RANDCacheSegment::clearCache() {
    for (auto i : container) {
        delete i.second;
    }
}

void RANDCacheSegment::deletePackage(std::string id) {
    if (container.find(id) != container.end()) {
        int freedSize = container[id]->getSize();
        cacheSize = cacheSize - freedSize;
        delete container[id];
        container.erase(id);
        keyList.erase(std::remove(keyList.begin(), keyList.end(), id),
                keyList.end());
    }
}

bool RANDCacheSegment::contains(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}
std::list<std::string>* RANDCacheSegment::insertIntoCache(VideoSegment* pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize >= maxCacheSize - pkg->getSize()) {
        int random = rand() % container.size();
        toDelete = keyList[random];
        deletePackage(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    container[keyBuilder] = pkg;
    keyList.push_back(keyBuilder);
    cacheSize = cacheSize + pkg->getSize();
    return deletedVideoSegments;
}

std::list<std::string>* RANDCacheSegment::reduce(int size) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    maxCacheSize = maxCacheSize - size;
    while (cacheSize > maxCacheSize - size) {
        int random = rand() % container.size();
        toDelete = keyList[random];
        deletePackage(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    writeOperation++;
    return deletedVideoSegments;
}

VideoSegment* RANDCacheSegment::retrieveSegment(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    return container[keyBuilder]->dup();
}
void RANDCacheSegment::expand(int i) {
    cacheSize = cacheSize + i;
}

int RANDCacheSegment::getReadOperations() {
    return this->readOperation;
}
int RANDCacheSegment::getWriteOperations() {
    return this->writeOperation;
}
std::string RANDCacheSegment::getCategory() {
    return this->category;
}
void RANDCacheSegment::rearrangeCache(VideoSegment* pkg) {

}
void RANDCacheSegment::setCategory(std::string category) {
    this->category = category;
}
