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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_LFU_LFUCACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_LFU_LFUCACHE_H_

#include <nodes/FrequencyNode.h>
#include <map>
#include <string>
#include "ReverseProxy.h"
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "BasicEvictionStrategy.h"

class LFUCache: public BasicEvictionStrategy {
public:
    LFUCache(std::vector<std::string>* parameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy);
    virtual ~LFUCache();
    void periodicEvents() override;
    void insertIntoCache(VideoSegment *pkg) override;
    bool contains(SegmentRequest *rqst) override;
    VideoSegment *retrieveSegment(SegmentRequest *rqst) override;
    long long getSize() override;
    void clearCache() override;
    int getReadOperations();
    int getWriteOperations();
    void deleteSegment(std::string id);
    void resetRates();
protected:
    bool expanded;/**< a boolean value that is true, if all cache expand or reduce operations have been performed */
    void setSize(long long size) override;
    void rearrangeCache(VideoSegment *pkg) override;
    std::string getLeastFrequent();
    std::map<std::string, std::pair<VideoSegment*, FrequencyNode*>*> container;/**< This models the storage of the cache */
    FrequencyNode* head;/**< The first node in the frequency node queue */
    unsigned long long maxCacheSize; /**< a vector storing all parameters for the eviction strategy */
    unsigned long long cacheSize = 0; /**< a long long representing the maximum size of the cache in kbit */
    std::vector<std::string>* parameters; /**< a vector storing all parameters for the eviction strategy, not used in LFU */
    int writeOperation = 0; /**< stores the write operations */
    int readOperation = 0; /**< stores the read operations */
    std::string expandStrat; /**< not used in FIFO */
    void alterCacheSize(double newCacheSize);
    std::vector<std::pair<double, double>>* storageAlterations;/**< a list of pairs of double values.
     the first value is the time of the alteration,
     the second value is the new maximum size of the cache.*/
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_LFU_LFUCACHE_H_ */
