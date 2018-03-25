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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_ACDC_ACDCCACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_ACDC_ACDCCACHE_H_
#include "BasicGhostList.h"
#include "BasicEvictionStrategy.h"
#include "BasicCacheSegment.h"
#include "ARCProbationaryCache.h"
#include <vector>
#include <string>
#include <omnetpp.h>
#include "BasicSSAS.h"

class ACARCCache: public BasicEvictionStrategy {
public:
    ACARCCache(std::vector<std::string>* parameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            const std::string& storageAlterationStrategy);
    virtual ~ACARCCache();
    void periodicEvents();
    void insertIntoCache(VideoSegment *pkg);
    bool contains(SegmentRequest *rqst);
    VideoSegment *retrievePackage(SegmentRequest *rqst);
    long long getSize();
    void clearCache();
    int getWriteOperations();
    int getReadOperations();
    void resetRates();
    void deletePackage(std::string id);
protected:
    bool expanded = false;
    std::string expandStrat = "";
    void setSize(long long size);
    std::vector<std::string>* parameters;
    unsigned long long maxCacheSize = 0;
    unsigned long long cacheSize = 0;
    void rearrangeCache(VideoSegment *pkg);
    long long subCacheSize = 0;
    ARCProbationaryCache* probationaryCache;
    omnetpp::cOutVector probationaryVector;
    omnetpp::cOutVector ghostListSize;
    BasicGhostList* probationaryGhostList;
    std::vector<BasicCacheSegment*> cacheSegmentVector;
    std::vector<omnetpp::cOutVector*> cacheSegemntSizes;
    std::vector<omnetpp::cOutVector*> ghostListSizes;
    std::vector<BasicGhostList*> ghostListVector;
    double minSegSize = 0.0;
    void setupCacheSegments();
    int findCacheForCategory(SegmentRequest* rqst);
    int findGhostListForCategory(VideoSegment* pkg);
    std::string adaptiveStrategy = "";
    void alterCacheSize(double newCacheSize);
    BasicSSAS* adaptionStrategy;
    std::vector<std::pair<double, double>>* storageAlterations;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_ACDC_ACDCCACHE_H_ */
