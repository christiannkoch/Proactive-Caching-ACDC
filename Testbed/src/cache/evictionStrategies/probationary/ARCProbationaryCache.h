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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_PROBATIONARY_ARCPROBATIONARYCACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_PROBATIONARY_ARCPROBATIONARYCACHE_H_
#include <nodes/RecencyNode.h>
#include <map>
#include <string>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "BasicProbationaryCache.h"


class ARCProbationaryCache : public BasicProbationaryCache {
public:
    ARCProbationaryCache(long long size);
    virtual ~ARCProbationaryCache();
    void setSize(long long size);
    long long getSize();
    long long getMaxSize();
    void periodicEvents();
    void clearCache();
    VideoSegment *retrievePackage(SegmentRequest *rqst);
    std::list<std::string>* insertIntoCache(VideoSegment *pkg);
    bool contains(SegmentRequest *rqst);
    void drop(VideoSegment* pkg);
    void expand(int i);
    std::list<std::string>* reduce(int i);
    int getReadOperations();
    int getWriteOperations();
    void deletePackage(std::string id);
    void resetRates();
protected:
    RecencyNode* head;
    std::map<std::string, std::pair<VideoSegment*,RecencyNode*>*> container;
    unsigned long long maxCacheSize;
    unsigned long long cacheSize = 0;
    void rearrangeCache(VideoSegment *pkg);
    int writeOperation = 0;
    int readOperation = 0;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_PROBATIONARY_ARCPROBATIONARYCACHE_H_ */
