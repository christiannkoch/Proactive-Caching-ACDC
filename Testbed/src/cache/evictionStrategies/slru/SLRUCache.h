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
/* @file SLRUCache.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the SLRUCache
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_SLRU_SLRUCACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_SLRU_SLRUCACHE_H_
#include <list>
#include <string>
#include <vector>
#include "../BasicEvictionStrategy.h"
#include "../segment/LRUCacheSegment.h"
#include "../probationary/SLRUProbationaryCache.h"
#include "../../../simulation/SegmentRequest_m.h"
#include "../../../simulation/VideoSegment_m.h"
#include "../ProbationaryCacheFactory.h"
#include "../CacheSegmentFactory.h"

class SLRUCache: public BasicEvictionStrategy {
public:
    SLRUCache(std::vector<std::string>* parameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy);
    virtual ~SLRUCache();
    void periodicEvents();
    void insertIntoCache(VideoSegment *pkg);
    bool contains(SegmentRequest *rqst);
    VideoSegment *retrieveSegment(SegmentRequest *rqst);
    long long getSize();
    void clearCache();
    int getWriteOperations();
    int getReadOperations();
    void deleteSegment(std::string id);
    void resetRates();
    std::string getCountsOfElements();
protected:
    bool expanded;
    void setSize(long long size);
    void rearrangeCache(VideoSegment *pkg);
    unsigned long long maxCacheSize;
    unsigned long long cacheSize;
    long long subCacheSize = 0;
    SLRUProbationaryCache* probationaryCache;
    LRUCacheSegment* protectedCache;
    std::vector<std::string>* parameters;
    std::string expandStrat;
    void alterCacheSize(double newCacheSize);
    std::vector<std::pair<double, double>>* storageAlterations;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_SLRU_SLRUCACHE_H_ */
