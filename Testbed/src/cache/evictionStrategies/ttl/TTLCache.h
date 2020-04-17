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
/* @file TTLCache.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the TTLCache
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_TTL_TTLCACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_TTL_TTLCACHE_H_
#include <nodes/RecencyNode.h>
#include <string>
#include <map>
#include <vector>
#include "BasicEvictionStrategy.h"
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "PointerAndCounter.h",

class TTLCache: public BasicEvictionStrategy {
public:
    TTLCache(std::vector<std::string>* parameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy);
    virtual ~TTLCache();
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
    RecencyNode* head;
    void setSize(long long size);
    std::vector<std::string>* parameters;
    unsigned int maxCacheSize;
    unsigned long long cacheSize = 0;
    void rearrangeCache(VideoSegment *pkg);
    std::map<std::string,
            std::pair<double, std::pair<PointerAndCounter*, RecencyNode*>*>*> container;
    int writeOperation = 0;
    int readOperation = 0;
    void purge();
    int timeToLive;
    std::string expandStrat;
    void alterCacheSize(double newCacheSize);
    std::vector<std::pair<double, double>>* storageAlterations;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_TTL_TTLCACHE_H_ */
