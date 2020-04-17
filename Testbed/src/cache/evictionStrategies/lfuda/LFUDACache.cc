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
/* @file LFUDACache.cc
 * @author Johannes Pfannmüller, Christian Koch
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
#include "LFUDACache.h"
#include <map>
#include <string>
#include <sstream>

/*
 * @brief Creates a new LFUDACache for caching functionalities
 * @param parameters the parameters for this eviction strategy
 * @param size The desired Size of this Cache
 * @param storageAlterations a vector of pairs of double value representing storage alterations
 * @param storageAlterationStrategy the strategy used when altering the size of the storage
 * @return A pointer to this class
 */
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
/**
 * @brief resets the cache hit and miss
 *
 * resets the values we use for tracking the performance of the caching strategy.
 * Also triggers the storage alteration if any exist
 */
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
/**
 * @brief deletes video segment from the storage of the cache
 *
 * When a video segment is deleted, we check if the corresponding frequency node
 * is now empty. if so, we update the pointers and delete the frequency node in
 * order to save storage.
 * @param id the video id of the video segment that has to be deleted
 */
void LFUDACache::deleteSegment(std::string id) {
    int freedSize = container[id]->first->getSegment()->getSize();
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
/**
 * @brief Inserts a video segment into the Cache
 *
 * A segment is inserted into the cache following the LFUDA algorithm
 * @param *pkg A VideoSegment
 */
void LFUDACache::insertIntoCache(VideoSegment *pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    while (cacheSize > maxCacheSize - pkg->getSize()) {
        std::string toDelete = getLeastFrequent();
        deleteSegment(toDelete);
    }
    if (nodeContainer.find(1 + globalK) == nodeContainer.end()) {
        std::set<std::string>* items = new std::set<std::string>();
        items->insert(keyBuilder);
        auto p = new std::pair<PointerAndCounter*, FrequencyNode*>(new PointerAndCounter(pkg, 0),
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
        auto p = new std::pair<PointerAndCounter*, FrequencyNode*>(new PointerAndCounter(pkg, 0),
                nodeContainer[1 + globalK]);
        container[keyBuilder] = p;
    }
    cacheSize = cacheSize + pkg->getSize();
    writeOperation++;
}

/*
 * @brief returns the predecessor to a key
 * @param key a key
 * @return the predecessor to a key
 */
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

/*
 * @brief returns the successor to a key
 * @param key a key
 * @return the successor to a key
 */
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

/*
 * @brief rearranges the cache on hit in the LFUDA manner
 *
 * @param pkg the video segment that triggered the rearrangement
 */
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
/**
 * @brief executes periodic events
 *
 * periodic events can regularily check on something
 */
void LFUDACache::periodicEvents() {

}
/**
 * @brief Checks if the requested segment is already in the Cache
 * @param rqst a segment request
 * @return A bool Value, indicating whether the Cache contains the segment or not
 */
bool LFUDACache::contains(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    if (container.find(keyBuilder) == container.end())
        return false;
    else
        return true;

}
/**
 * @brief Retrieves the video segment from the Cache. This should only be executed, if contains returns true.
 * @param rqst A segment request
 *
 * The video segment is returned and we call rearrangeCache.
 * @return The video segment that fullfills the segment request
 *
 */
VideoSegment *LFUDACache::retrieveSegment(SegmentRequest *rqst) {
    readOperation++;
    std::string keyBuilder = rqst->getVideoId()
            + std::to_string(rqst->getSegmentId());
    VideoSegment *pkg = container[keyBuilder]->first->getSegment();
    container[keyBuilder]->first->increaseCount();
    rearrangeCache(pkg);
    return pkg->dup();
}
/**
 * @brief Sets the size of the Cache
 * @param size An Integer Value
 */
void LFUDACache::setSize(long long size) {
    maxCacheSize = size;
}
/**
 * @brief Get the size of the cache
 * @return Returns an integer Value describing the size of the cache
 */
long long LFUDACache::getSize() {
    return this->cacheSize;
}
/**
 * @brief deletes all Objects in the Cache
 */
void LFUDACache::clearCache() {

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

/*
 * @brief returns the id of the least frequently used video segment
 * @return the id of the least frequently used video segment
 */
std::string LFUDACache::getLeastFrequent() {
    return *head->getNext()->getItems()->begin();
}
/**
 * @brief alters the cache size
 *
 * a function that alters the maximum size of the cache to a given double value
 *
 * @param newCacheSize a double value representing the new size of the cache
 */
void LFUDACache::alterCacheSize(double newCacheSize) {
    maxCacheSize = newCacheSize;
}
/**
 * @brief returns the read operations
 *
 * Returns the amount of read operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of read operations performed
 */
int LFUDACache::getReadOperations() {
    return this->readOperation;
}
/**
 * @brief returns the write operations
 *
 * Returns the amount of write operations performed since the last reset of the rates
 * @return an Integer Value representing the amount of write operations performed
 */
int LFUDACache::getWriteOperations() {
    return this->writeOperation;
}

std::string LFUDACache::getCountsOfElements(){
    std::stringstream buf;
    for (auto i : container){
        buf << i.first << ", " << i.second->first->getCount() << "; ";
    }

    return buf.str();
}

