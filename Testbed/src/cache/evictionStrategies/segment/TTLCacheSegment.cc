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

#include "TTLCacheSegment.h"
#include <string>
#include <map>

TTLCacheSegment::TTLCacheSegment(long long size, std::string category) {
    // TODO Auto-generated constructor stub
    setSize(size);
    setCategory(category);
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
}

TTLCacheSegment::~TTLCacheSegment() {
    // TODO Auto-generated destructor stub
}

void TTLCacheSegment::resetRates() {
    this->writeOperation = 0;
    this->readOperation = 0;
}

void TTLCacheSegment::setSize(long long size) {
    this->cacheSize = size;
}
long long TTLCacheSegment::getSize() {
    return this->cacheSize;
}
long long TTLCacheSegment::getMaxSize(){
    return this->maxCacheSize;
}
void TTLCacheSegment::periodicEvents() {
    purge();
}
void TTLCacheSegment::clearCache() {
    for (auto i : container) {
        delete i.second->second->first;
        delete i.second->second->second;
        delete i.second->second;
    }
    delete head;
}
bool TTLCacheSegment::contains(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}

void TTLCacheSegment::deletePackage(std::string id) {
    if (container.find(id) != container.end()) {
        RecencyNode* rec = container[id]->second->second;
        delete container[id]->second->first;
        rec->getPrev()->setNext(rec->getNext());
        rec->getNext()->setPrev(rec->getPrev());
        delete rec;
        delete container[id]->second;
        delete container[id];
        container.erase(id);
        writeOperation++;
    }
}

std::list<std::string>* TTLCacheSegment::insertIntoCache(VideoSegment* pkg) {
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
    auto k = new std::pair<double, std::pair<VideoSegment*, RecencyNode*>*>(
            omnetpp::simTime().dbl(), p);
    container[keyBuilder] = k;
    head->getNext()->setPrev(p->second);
    head->setNext(p->second);
    writeOperation++;
    cacheSize = cacheSize + pkg->getSize();
    return deletedVideoSegments;
}

std::list<std::string>* TTLCacheSegment::reduce(int size) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - size) {
        toDelete = head->getPrev()->getValue();
        deletePackage(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    writeOperation++;
    return deletedVideoSegments;
}

VideoSegment* TTLCacheSegment::retrieveSegment(SegmentRequest* rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->second->first;
    container[keyBuilder]->first = omnetpp::simTime().dbl();
    rearrangeCache(pkg);
    return pkg->dup();
}
void TTLCacheSegment::expand(int i) {
    cacheSize = cacheSize + 1;
}

int TTLCacheSegment::getReadOperations() {
    return this->readOperation;
}
int TTLCacheSegment::getWriteOperations() {
    return this->writeOperation;
}
std::string TTLCacheSegment::getCategory() {
    return this->category;
}
void TTLCacheSegment::rearrangeCache(VideoSegment* pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    RecencyNode* rec = container[keyBuilder]->second->second;
    RecencyNode* next_rec = rec->getNext();

    rec->getPrev()->setNext(next_rec);
    next_rec->setPrev(rec->getPrev());
    rec->setNext(head->getNext());
    rec->getNext()->setPrev(rec);
    rec->setPrev(head);
    head->setNext(rec);
}
void TTLCacheSegment::setCategory(std::string category) {
    this->category = category;
}

void TTLCacheSegment::purge() {
    for (auto i : container) {
        if ((omnetpp::simTime().dbl() - i.second->first) > timeToLive) {
            std::string toDelete = i.first;
            deletePackage(toDelete);
        }
        readOperation++;
    }
}
