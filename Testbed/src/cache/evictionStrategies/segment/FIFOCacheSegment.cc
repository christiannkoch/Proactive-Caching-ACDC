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

#include "FIFOCacheSegment.h"

FIFOCacheSegment::FIFOCacheSegment(long long size, std::string category) {
    // TODO Auto-generated constructor stub
    setSize(size);
    setCategory(category);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);

}

FIFOCacheSegment::~FIFOCacheSegment() {
    // TODO Auto-generated destructor stub
}

void FIFOCacheSegment::periodicEvents() {

}

void FIFOCacheSegment::resetRates() {
    this->readOperation = 0;
    this->writeOperation = 0;
}

void FIFOCacheSegment::deletePackage(std::string id) {
    if (container.find(id) != container.end()) {
        int freedSize = container[id]->first->getSize();
        RecencyNode* rec = container[id]->second;
        rec->getPrev()->setNext(rec->getNext());
        rec->getNext()->setPrev(rec->getPrev());
        delete container[id]->first;
        delete rec;
        delete container[id];
        container.erase(id);
        cacheSize = cacheSize - freedSize;
        writeOperation++;
    }

}

std::list<std::string>* FIFOCacheSegment::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        toDelete = head->getPrev()->getValue();
        deletePackage(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    auto p = new std::pair<VideoSegment*, RecencyNode*>(pkg,
            new RecencyNode(keyBuilder, head, head->getNext()));
    container[keyBuilder] = p;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    writeOperation++;
    cacheSize = cacheSize + pkg->getSize();
    return deletedVideoSegments;
}

std::list<std::string>* FIFOCacheSegment::reduce(int size) {
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

bool FIFOCacheSegment::contains(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}

VideoSegment* FIFOCacheSegment::retrievePackage(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
    rearrangeCache(pkg);
    return pkg->dup();
}

void FIFOCacheSegment::setSize(long long size) {
    this->maxCacheSize = size;
}

long long FIFOCacheSegment::getMaxSize(){
    return this->maxCacheSize;
}

long long FIFOCacheSegment::getSize() {
    return this->cacheSize;
}

void FIFOCacheSegment::clearCache() {
    for (auto i : container) {
        delete i.second->first;
        delete i.second->second;
        delete i.second;
    }
    delete head;
}

void FIFOCacheSegment::rearrangeCache(VideoSegment *pkg) {

}

int FIFOCacheSegment::getReadOperations() {
    return this->readOperation;
}
int FIFOCacheSegment::getWriteOperations() {
    return this->writeOperation;
}

void FIFOCacheSegment::expand(int i) {
    maxCacheSize = maxCacheSize + i;
}

void FIFOCacheSegment::setCategory(std::string category) {
    this->category = category;
}

std::string FIFOCacheSegment::getCategory() {
    return this->category;
}
