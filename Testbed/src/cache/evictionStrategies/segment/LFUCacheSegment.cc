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
#include <nodes/FrequencyNode.h>
#include "LFUCacheSegment.h"
#include <map>
#include <string>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

LFUCacheSegment::LFUCacheSegment(long long size, std::string category) {
    // TODO Auto-generated constructor stub
    setSize(size);
    setCategory(category);
    auto f = new FrequencyNode(0, nullptr, nullptr, nullptr);
    f->setNext(f);
    f->setPrev(f);
    this->head = f;
}

LFUCacheSegment::~LFUCacheSegment() {
    // TODO Auto-generated destructor stub
}

void LFUCacheSegment::resetRates() {
    this->writeOperation = 0;
    this->readOperation = 0;
}

void LFUCacheSegment::periodicEvents() {
}

void LFUCacheSegment::rearrangeCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    FrequencyNode* freq = container[keyBuilder]->second;
    FrequencyNode* next_freq = freq->getNext();

    if ((next_freq->getValue() == 0)
            || (next_freq->getValue() != (freq->getValue() + 1))) {
        std::set<std::string>* items = new std::set<std::string>();
        items->insert(keyBuilder);
        FrequencyNode* newNext_freq = new FrequencyNode(freq->getValue() + 1,
                items, freq, next_freq);
        next_freq->setPrev(newNext_freq);
        freq->setNext(newNext_freq);
    } else {
        next_freq->getItems()->insert(keyBuilder);
    }
    freq->getItems()->erase(keyBuilder);
    container[keyBuilder]->second = freq->getNext();

    if (freq->getItems()->size() == 0) {
        freq->getPrev()->setNext(freq->getNext());
        freq->getNext()->setPrev(freq->getPrev());
        //frequencyList.erase(freq);
        delete freq;
    }
}

void LFUCacheSegment::setCategory(std::string category) {
    this->category = category;
}

std::string LFUCacheSegment::getCategory() {
    return this->category;
}

void LFUCacheSegment::setSize(long long size) {
    this->maxCacheSize = size;
}

long long LFUCacheSegment::getSize() {
    return this->cacheSize;
}

long long LFUCacheSegment::getMaxSize(){
    return this->maxCacheSize;
}

void LFUCacheSegment::clearCache() {
    for (auto i : container) {
        std::string toDelete = i.first;
        delete (container[toDelete]->first);
        delete (container[toDelete]);
    }

    FrequencyNode* freq = head->getNext();
    while (freq->getValue() != 0) {
        freq = freq->getNext();
        delete freq->getPrev();
    }

    delete head;
}

bool LFUCacheSegment::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;
}

void LFUCacheSegment::deletePackage(std::string id) {
    if (container.find(id) != container.end()) {
        FrequencyNode* freq = container[id]->second;
        freq->getItems()->erase(id);
        if (freq->getItems()->size() == 0) {
            freq->getPrev()->setNext(freq->getNext());
            freq->getNext()->setPrev(freq->getPrev());
            delete (freq);

        }
        int freedSize = container[id]->first->getSize();
        cacheSize = cacheSize - freedSize;
        delete (container[id]->first);
        delete (container[id]);
        container.erase(id);
        writeOperation++;
    }
}

std::list<std::string>* LFUCacheSegment::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        toDelete = getLeastFrequent();
        deletePackage(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    FrequencyNode* freq = head->getNext();
    if (freq->getValue() != 1) {
        std::set<std::string>* items = new std::set<std::string>();
        items->insert(keyBuilder);
        auto p = new std::pair<VideoSegment*, FrequencyNode*>(pkg,
                new FrequencyNode(1, items, head, freq));
        container[keyBuilder] = p;
        freq->setPrev(p->second);
        head->setNext(p->second);

    } else {
        freq->getItems()->insert(keyBuilder);
        auto p = new std::pair<VideoSegment*, FrequencyNode*>(pkg, freq);
        container[keyBuilder] = p;
    }
    writeOperation++;
    cacheSize = cacheSize + pkg->getSize();
    return deletedVideoSegments;
}

std::list<std::string>* LFUCacheSegment::reduce(int size) {
    std::string toDelete = "";
    std::list<std::string>* deletedVideoSegments = new std::list<std::string>();
    maxCacheSize = maxCacheSize - size;
    while (cacheSize > maxCacheSize - size) {
        toDelete = getLeastFrequent();
        deletePackage(toDelete);
        deletedVideoSegments->push_back(toDelete);
    }
    writeOperation++;
    return deletedVideoSegments;
}

VideoSegment *LFUCacheSegment::retrievePackage(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
    rearrangeCache(pkg);
    return pkg->dup();
}

void LFUCacheSegment::expand(int i) {
    maxCacheSize = maxCacheSize + i;
}

std::string LFUCacheSegment::getLeastFrequent() {
    return *head->getNext()->getItems()->begin();
}

int LFUCacheSegment::getReadOperations() {
    return this->readOperation;
}
int LFUCacheSegment::getWriteOperations() {
    return this->writeOperation;
}
