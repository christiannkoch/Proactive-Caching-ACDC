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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_LRUCACHESEGMENT_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_LRUCACHESEGMENT_H_

#include <nodes/RecencyNode.h>
#include <map>
#include <string>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "BasicCacheSegment.h"
#include "PointerAndCounter.h"

class LRUCacheSegment: public BasicCacheSegment {
public:
    LRUCacheSegment(long long size, std::string category);
    virtual ~LRUCacheSegment();
    void setSize(long long size);
    long long getSize();
    long long getMaxSize();
    void periodicEvents();
    void clearCache();
    bool contains(SegmentRequest* rqst);
    std::list<std::string>* insertIntoCache(VideoSegment* pkg);
    VideoSegment* retrieveSegment(SegmentRequest* rqst);
    void expand(int i);
    std::list<std::string>* reduce(int i);
    int getReadOperations();
    int getWriteOperations();
    std::string getCategory();
    VideoSegment* getElementWithSecondChance();
    void deleteSegment(std::string id);
    void resetRates();
    std::string getCountsOfElements();
protected:
    unsigned long long maxCacheSize;
        unsigned long long cacheSize = 0;
    void rearrangeCache(VideoSegment* pkg);
    std::map<std::string, std::pair<PointerAndCounter*, RecencyNode*>*> container;
    void setCategory(std::string category);
    RecencyNode* head;
    std::string category;
    int writeOperation = 0;
    int readOperation = 0;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_LRUCACHESEGMENT_H_ */
