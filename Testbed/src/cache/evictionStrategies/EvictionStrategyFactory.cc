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
/* @file EvictionStrategyFactory.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief a factory for the desired eviction strategy
 *
 * @section DESCRIPTION
 *
 * returns the desired eviction strategy
 */
#include "EvictionStrategyFactory.h"
#include <string>
#include <vector>

#include "ACDCCache.h"
#include "ARC1Cache.h"
#include "FIFOCache.h"
#include "LFUCache.h"
#include "LFUDACache.h"
#include "LRUCache.h"
#include "RANDCache.h"
#include "SLRUCache.h"
#include "TTLCache.h"
#include "ARC2Cache.h"
#include "LFUDACache.h"
#include "InfinityCache.h"

EvictionStrategyFactory::EvictionStrategyFactory() {
    // TODO Auto-generated constructor stub
}
/** @brief creates the desired eviction strategy
 *
 * @param implementation a string representing the type of eviction strategy
 *
 * @param cModule The Reverseproxy this eviction strategy belongs to (maybe obsolete)
 *
 * @param cacheSize The size of the cache
 *
 * @return a pointer to a evictionStrategy
 */
BasicEvictionStrategy* EvictionStrategyFactory::createEvictionStrategy(
        std::string implementation, std::vector<std::string>* parameters,
        long long size, std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy) {
    if (implementation == "ACDC")
        return new ACDCCache(parameters, size, storageAlterations,
                storageAlterationStrategy.c_str());
    else if (implementation == "ARC1")
        return new ARC1Cache(parameters, size, storageAlterations,
                storageAlterationStrategy);
    else if (implementation == "ARC2")
        return new ARC2Cache(parameters, size, storageAlterations,
                storageAlterationStrategy);
    else if (implementation == "FIFO")
        return new FIFOCache(parameters, size, storageAlterations,
                storageAlterationStrategy);
    else if (implementation == "LFU")
        return new LFUCache(parameters, size, storageAlterations,
                storageAlterationStrategy);
    else if (implementation == "LFUDA")
        return new LFUDACache(parameters, size, storageAlterations,
                storageAlterationStrategy);
    else if (implementation == "LRU")
        return new LRUCache(parameters, size, storageAlterations,
                storageAlterationStrategy);
    else if (implementation == "RAND")
        return new RANDCache(parameters, size, storageAlterations,
                storageAlterationStrategy);
    else if (implementation == "SLRU")
        return new SLRUCache(parameters, size, storageAlterations,
                storageAlterationStrategy);
    else if (implementation == "TTL")
        return new TTLCache(parameters, size, storageAlterations,
                storageAlterationStrategy);
    else if (implementation == "INFINITY")
        return new InfinityCache(parameters, size, storageAlterations,
                storageAlterationStrategy);
    else
        throw "This type of eviction Strategy does not exist\n";
}
