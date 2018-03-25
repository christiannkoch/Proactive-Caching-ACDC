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
#include "ARCProbationaryCache.h"
#include <omnetpp.h>
#include <list>
#include <string>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

ARCProbationaryCache::ARCProbationaryCache(long long size) {
    // TODO Auto-generated constructor stub
    setSize(size);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
}

ARCProbationaryCache::~ARCProbationaryCache() {
    // TODO Auto-generated destructor stub
}

void ARCProbationaryCache::resetRates() {
    this->readOperation = 0;
    this->writeOperation = 0;
}

void ARCProbationaryCache::deletePackage(std::string id) {
    if (container.find(id) != container.end()) {
        int freedSize = container[id]->first->getSize();
        RecencyNode* rec = container[id]->second;
        delete container[id]->first;
        rec->getPrev()->setNext(rec->getNext());
        rec->getNext()->setPrev(rec->getPrev());
        delete rec;
        delete container[id];
        container.erase(id);
        cacheSize = cacheSize - freedSize;
        writeOperation++;
    }
}

std::list<std::string>* ARCProbationaryCache::insertIntoCache(VideoSegment *pkg) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while(cacheSize > maxCacheSize - pkg->getSize()) {
            toDelete = head->getPrev()->getValue();
            deletePackage(toDelete);
            deletedVideoSegments->push_back(toDelete);
        }
    auto p = new std::pair<VideoSegment*, RecencyNode*>(pkg,
            new RecencyNode(keyBuilder, head, head->getNext()));
    container[keyBuilder] = p;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
    return deletedVideoSegments;

}

std::list<std::string>* ARCProbationaryCache::reduce(int size) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    maxCacheSize = maxCacheSize - size;
    while (cacheSize > maxCacheSize - size) {
        toDelete = head->getPrev()->getValue();
        deletePackage(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    writeOperation++;
    return deletedVideoSegments;
}

void ARCProbationaryCache::drop(VideoSegment* pkg) {
    std::string toDelete = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    deletePackage(toDelete);
}

void ARCProbationaryCache::setSize(long long size) {
    this->maxCacheSize = size;
}

long long ARCProbationaryCache::getSize() {
    return this->cacheSize;
}

long long ARCProbationaryCache::getMaxSize(){
    return this->maxCacheSize;
}

VideoSegment* ARCProbationaryCache::retrievePackage(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
    return pkg->dup();
}

bool ARCProbationaryCache::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}



void ARCProbationaryCache::periodicEvents() {
}

void ARCProbationaryCache::clearCache() {
    for (auto i : container) {
        delete i.second->first;
        delete i.second->second;
        delete i.second;
    }
    delete head;
}

void ARCProbationaryCache::expand(int i) {
    maxCacheSize = maxCacheSize + i;
}

int ARCProbationaryCache::getReadOperations() {
    return this->readOperation;
}
int ARCProbationaryCache::getWriteOperations() {
    return this->writeOperation;
}
