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
 * @brief Implements the Least Frequently Used with Dynamic Aging Eviction Strategy
 *
 * @section DESCRIPTION
 *
 * This Class is responsible for managing the storage with the behaviour of the LFUDA
 * Eviction Strategy
 *
 */
#include <evictionStrategies/lfuda/LFUDACache.h>
#include <nodes/FrequencyNode.h>
#include <map>
#include <string>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

LFUDACache::LFUDACache(std::vector<std::string>* parameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy) {
    this->parameters = parameters;
    setSize(size);
    auto f = new FrequencyNode(0, nullptr, nullptr, nullptr);
    f->setNext(f);
    f->setPrev(f);
    this->head = f;
    this->globalK = 0;
    expandStrat = storageAlterationStrategy;
    this->storageAlterations = storageAlterations;
    if (storageAlterations->size() > 0) {
        expanded = false;
    } else
        expanded = true;
}

LFUDACache::~LFUDACache() {
    delete storageAlterations;
}

void LFUDACache::resetRates() {
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

void LFUDACache::deletePackage(std::string id) {
    int freedSize = container[id]->first->getSize();
    FrequencyNode* freq = container[id]->second;
    freq->getItems()->erase(id);
    globalK = freq->getValue();
    if (freq->getItems()->size() == 0) {
        freq->getPrev()->setNext(freq->getNext());
        freq->getNext()->setPrev(freq->getPrev());
        nodeContainer.erase(freq->getValue());
        delete (freq);
        writeOperation++;
    }
    delete (container[id]->first);
    delete (container[id]);
    container.erase(id);
    cacheSize = cacheSize - freedSize;
    readOperation++;
    writeOperation++;
}

void LFUDACache::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        std::string toDelete = getLeastFrequent();
        deletePackage(toDelete);
    }
    if (nodeContainer.find(1 + globalK) == nodeContainer.end()) {
        std::set<std::string>* items = new std::set<std::string>();
        items->insert(keyBuilder);
        auto p = new std::pair<VideoSegment*, FrequencyNode*>(pkg,
                new FrequencyNode(1 + globalK, items, nullptr, nullptr));
        container[keyBuilder] = p;
        nodeContainer[1 + globalK] = p->second;
        FrequencyNode* pred = findPred(1 + globalK);
        FrequencyNode* succ = pred->getNext();
        p->second->setNext(succ);
        p->second->setPrev(pred);
        succ->setPrev(p->second);
        pred->setNext(p->second);

    } else {
        nodeContainer[1 + globalK]->getItems()->insert(keyBuilder);
        auto p = new std::pair<VideoSegment*, FrequencyNode*>(pkg,
                nodeContainer[1 + globalK]);
        container[keyBuilder] = p;
    }
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
}

FrequencyNode* LFUDACache::findPred(int key) {
    if (head->getNext() == head)
        return head;
    if (head->getNext()->getValue() > key)
        return head;
    FrequencyNode* freq = head->getNext();
    while (freq != head) {
        if (freq->getNext()->getValue() > key || freq->getNext() == head)
            return freq;
        freq = freq->getNext();
    }
    return nullptr;
}

FrequencyNode* LFUDACache::findSucc(int key) {
    if (head->getNext() == head)
        return head;
    if (head->getPrev()->getValue() < key)
        return head;
    FrequencyNode* freq = head->getPrev();
    while (freq != head) {
        if (freq->getPrev()->getValue() < key || freq->getPrev() == head)
            return freq;
        freq = freq->getPrev();
    }
    return nullptr;
}

void LFUDACache::rearrangeCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    FrequencyNode* freq = container[keyBuilder]->second;
    FrequencyNode* next_freq = freq->getNext();

    if ((next_freq->getValue() == 0)
            || (nodeContainer.find(freq->getValue() + 1 + globalK)
                    == nodeContainer.end())) {
        std::set<std::string>* items = new std::set<std::string>();
        items->insert(keyBuilder);
        FrequencyNode* newNext_freq = new FrequencyNode(
                freq->getValue() + 1 + globalK, items, nullptr, nullptr);
        container[keyBuilder]->second = newNext_freq;
        nodeContainer[freq->getValue() + 1 + globalK] = newNext_freq;
        FrequencyNode* pred = findPred(freq->getValue() + 1 + globalK);
        FrequencyNode* succ = pred->getNext();
        newNext_freq->setNext(succ);
        newNext_freq->setPrev(pred);
        succ->setPrev(newNext_freq);
        pred->setNext(newNext_freq);
    } else {
        nodeContainer[freq->getValue() + 1 + globalK]->getItems()->insert(
                keyBuilder);
    }
    freq->getItems()->erase(keyBuilder);
    container[keyBuilder]->second =
            nodeContainer[freq->getValue() + 1 + globalK];

    if (freq->getItems()->size() == 0) {
        freq->getPrev()->setNext(freq->getNext());
        freq->getNext()->setPrev(freq->getPrev());
        nodeContainer.erase(freq->getValue());
        delete freq;
    }
}

void LFUDACache::periodicEvents() {

}

bool LFUDACache::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;

}

VideoSegment *LFUDACache::retrievePackage(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first;
    rearrangeCache(pkg);
    return pkg->dup();
}

void LFUDACache::setSize(long long size) {
    maxCacheSize = size;
}

long long LFUDACache::getSize() {
    return this->cacheSize;
}

void LFUDACache::clearCache() {
    /*for (auto iter : container) {
     delete (iter.second);
     }*/
    //delete head;
    for (auto i : container) {
        delete i.second->first;
        delete i.second;
    }
    FrequencyNode* freq = head->getNext();
    while (freq->getValue() != 0) {
        freq = freq->getNext();
        delete freq->getPrev();
    }

    delete head;
}

std::string LFUDACache::getLeastFrequent() {
    return *head->getNext()->getItems()->begin();
}

void LFUDACache::alterCacheSize(double newCacheSize) {
    maxCacheSize = newCacheSize;
}

int LFUDACache::getReadOperations() {
    return this->readOperation;
}
int LFUDACache::getWriteOperations() {
    return this->writeOperation;
}
