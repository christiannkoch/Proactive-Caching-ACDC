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

#include "SLRUProbationaryCache.h"
#include <list>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

SLRUProbationaryCache::SLRUProbationaryCache(long long size) {
    // TODO Auto-generated constructor stub
    setSize(size);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
}

SLRUProbationaryCache::~SLRUProbationaryCache() {
    // TODO Auto-generated destructor stub
}

void SLRUProbationaryCache::resetRates() {
    this->readOperation = 0;
    this->writeOperation = 0;
}

void SLRUProbationaryCache::periodicEvents() {

}

void SLRUProbationaryCache::deletePackage(std::string id) {
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

std::list<std::string>* SLRUProbationaryCache::insertIntoCache(VideoSegment *pkg) {

    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize >= maxCacheSize - pkg->getSize()) {
        std::string toDelete = head->getPrev()->getValue();
        deletePackage(toDelete);
    }
    auto p = new std::pair<VideoSegment*, RecencyNode*>(pkg,
            new RecencyNode(keyBuilder, head, head->getNext()));
    container[keyBuilder] = p;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
    return nullptr;
}

void SLRUProbationaryCache::drop(VideoSegment* pkg) {
    std::string toDelete = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    deletePackage(toDelete);
}

void SLRUProbationaryCache::insertIntoCacheAtEnd(VideoSegment* pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize > maxCacheSize - pkg->getSize()) {
            std::string toDelete = head->getPrev()->getValue();
            deletePackage(toDelete);
        }
    auto p = new std::pair<VideoSegment*, RecencyNode*>(pkg,
            new RecencyNode(keyBuilder, head->getPrev(), head));
    container[keyBuilder] = p;
    head->getPrev()->setNext(p->second);
    head->setPrev(p->second);
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
}

bool SLRUProbationaryCache::contains(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}

VideoSegment* SLRUProbationaryCache::retrievePackage(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
    return pkg->dup();
}

void SLRUProbationaryCache::setSize(long long size) {
    this->maxCacheSize = size;
}

long long SLRUProbationaryCache::getSize() {
    return this->cacheSize;
}

long long SLRUProbationaryCache::getMaxSize() {
    return this->maxCacheSize;
}

void SLRUProbationaryCache::clearCache() {
    for (auto i : container) {
        delete i.second->first;
        delete i.second->second;
        delete i.second;
    }
    delete head;
}

int SLRUProbationaryCache::getReadOperations() {
    return this->readOperation;
}
int SLRUProbationaryCache::getWriteOperations() {
    return this->writeOperation;
}
