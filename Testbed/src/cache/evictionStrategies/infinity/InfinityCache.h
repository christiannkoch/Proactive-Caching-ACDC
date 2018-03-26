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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_INFINITY_INFINITYCACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_INFINITY_INFINITYCACHE_H_

#include <omnetpp.h>
#include <map>
#include <string>
#include <vector>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "BasicEvictionStrategy.h"

class InfinityCache: public BasicEvictionStrategy {
public:
    InfinityCache(std::vector<std::string>* parameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy);
    virtual ~InfinityCache();
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
protected:
    void setSize(long long size);
    std::map<std::string, std::string> container;
    unsigned long long maxCacheSize;
    unsigned long long cacheSize = 0;
    void rearrangeCache(VideoSegment *pkg);
    std::vector<std::string>* parameters;
    int writeOperation = 0;
    int readOperation = 0;
    std::string expandStrat;
    void alterCacheSize(double newCacheSize);
    std::vector<std::pair<double, double>>* storageAlterations;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_INFINITY_INFINITYCACHE_H_ */
