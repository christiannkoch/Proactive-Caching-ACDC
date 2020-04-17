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
/* @file FIFOCache.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the FIFOCache
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_FIFO_FIFOCACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_FIFO_FIFOCACHE_H_


#include <map>
#include <string>
#include <vector>
#include "../BasicEvictionStrategy.h"
#include "../nodes/RecencyNode.h"
#include "../../PointerAndCounter.h"

class FIFOCache: public BasicEvictionStrategy {
public:

    FIFOCache(std::vector<std::string>* parameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy);
    virtual ~FIFOCache();
    void periodicEvents();
    void insertIntoCache(VideoSegment *pkg);
    bool contains(SegmentRequest *rqst);
    VideoSegment *retrieveSegment(SegmentRequest *rqst);
    long long getSize();
    void clearCache();
    int getReadOperations();
    int getWriteOperations();
    void deleteSegment(std::string id);
    void resetRates();
    std::string getCountsOfElements();
protected:
    bool expanded;/**< a boolean value that is true, if all cache expand or reduce operations have been performed */
    RecencyNode* head;
    void setSize(long long size);
    std::map<std::string, std::pair<PointerAndCounter*, RecencyNode*>*> container;/**< This models the storage of the cache */
    unsigned long long maxCacheSize; /**< a vector storing all parameters for the eviction strategy */
    unsigned long long cacheSize = 0; /**< a long long representing the maximum size of the cache in kbit */
    void rearrangeCache(VideoSegment *pkg);
    std::vector<std::string>* parameters; /**< a vector storing all parameters for the eviction strategy, not used in FIFO */
    int writeOperation = 0; /**< stores the write operations */
    int readOperation = 0;/**< stores the read operations */
    std::string expandStrat; /**< not used in FIFO */
    void alterCacheSize(double newCacheSize);
    std::vector<std::pair<double, double>>* storageAlterations;/**< a list of pairs of double values.
     the first value is the time of the alteration,
     the second value is the new maximum size of the cache.*/
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_FIFO_FIFOCACHE_H_ */
