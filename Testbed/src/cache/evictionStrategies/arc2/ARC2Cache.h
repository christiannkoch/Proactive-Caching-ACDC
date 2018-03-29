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
/* @file ARC2Cache.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the ARC2Cache
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_ARC2_ARC2CACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_ARC2_ARC2CACHE_H_
#include <ghostLists/LimitedGhostList.h>
#include <omnetpp.h>
#include <map>
#include <string>
#include <vector>
#include "ARCProbationaryCache.h"
#include "BasicEvictionStrategy.h"
#include "LRUCacheSegment.h"
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "ReverseProxy.h"

class ARC2Cache: public BasicEvictionStrategy {
public:

    ARC2Cache(std::vector<std::string>* parameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy);
    virtual ~ARC2Cache();
    void periodicEvents();
    void insertIntoCache(VideoSegment *pkg);
    bool contains(SegmentRequest *rqst);
    VideoSegment *retrieveSegment(SegmentRequest *rqst);
    long long getSize();
    void clearCache();
    void resetRates();
    void deleteSegment(std::string id);
    int getWriteOperations();
    int getReadOperations();
protected:
    void setSize(long long size);
    bool expanded; /**< a boolean value that is true, if all cache expand or reduce operations have been performed */
    std::vector<std::string>* parameters; /**< a vector storing all parameters for the eviction strategy */
    unsigned long long maxCacheSize; /**< a long long representing the maximum size of the cache in kbit */
    unsigned long long cacheSize = 0; /**< a long long representing the current size of the cache in kbit */
    void rearrangeCache(VideoSegment *pkg); // wird in retrieveResponse aufgerufen
    long long subCacheSize; /**< a long long representing the maximum size of the subcaches */
    ARCProbationaryCache* t1; /**< the probationary cache */
    LRUCacheSegment* t2; /**< the lru cache segment */
    LimitedGhostList* b1; /**< the ghost list for the probationary cache */
    LimitedGhostList* b2; /**< the ghost list for the lru cache segment*/
    std::string expandStrat; /**<the strategy used when and if the maximum cache size is altered */
    void alterCacheSize(double newCacheSize);
    std::vector<std::pair<double, double>>* storageAlterations; /**< a list of pairs of double values.
     the first value is the time of the alteration,
     the second value is the new maximum size of the cache.*/
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_ARC2_ARC2CACHE_H_ */
