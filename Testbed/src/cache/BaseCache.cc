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

#include "BaseCache.h"
/*
 * @brief the constructor for the BaseCache class
 *
 * A caching strategy consists of an admission and an eviction strategy. This is the constructor for the BaseCache class.
 * It takes several parameters and creates a basic cache consisting of an admission strategy, an eviction strategy and
 * the reverse proxy to which it belongs.
 *
 * @param admission The admission strategy of the cache
 *
 * @param eviction The eviction strategy of the cache
 *
 * @param proxy The reverse proxy to which this cache belongs
 */
BaseCache::BaseCache(BasicAdmissionStrategy* admission,
        BasicEvictionStrategy* eviction, omnetpp::cModule* reverseProxy) {
    // TODO Auto-generated constructor stub
    this->admission = admission;
    this->eviction = eviction;
    this->proxy = reverseProxy;
}

BaseCache::~BaseCache() {
    delete admission;
    delete eviction;
}

/**
 * @brief returns a pointer to the admission strategy
 *
 * @return a pointer to the admission strategy of the cache
 */
BasicAdmissionStrategy* BaseCache::getAdmissionStrategy() {
    return this->admission;
}

/**
 * @brief returns a pointer to the eviction strategy
 *
 * @return a pointer to the eviction strategy of the cache
 */
BasicEvictionStrategy* BaseCache::getEvictionStrategy() {
    return this->eviction;
}

/**
 * @brief Returns the number of read operations.
 *
 * The Number of read operations that were performed since the last call of the function are returned.
 *
 * @return an integer number representing the amount of read operations done
 */
int BaseCache::getReadOperations() {
    return eviction->getReadOperations();
}

/**
 * @brief Returns the number of write operations.
 *
 * The Number of write operations that were performed since the last call of the function are returned.
 *
 * @return an integer number representing the amount of write operations done
 */
int BaseCache::getWriteOperations() {
    return eviction->getWriteOperations();
}
