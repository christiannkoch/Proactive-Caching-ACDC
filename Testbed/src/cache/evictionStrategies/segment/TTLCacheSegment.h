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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_TTLCACHESEGMENT_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_TTLCACHESEGMENT_H_
#include <nodes/RecencyNode.h>
#include <string>
#include <map>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "BasicCacheSegment.h"

class TTLCacheSegment: public BasicCacheSegment {
public:
    TTLCacheSegment(long long size, std::string category);
    virtual ~TTLCacheSegment();
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
    void deleteSegment(std::string id);
    void resetRates();
protected:
    RecencyNode* head;
    unsigned long long maxCacheSize;
        unsigned long long cacheSize;
    void rearrangeCache(VideoSegment* pkg);
    void setCategory(std::string category);
    std::map<std::string, std::pair<double, std::pair<VideoSegment*,RecencyNode*>*>*> container;
    std::string category;
    int writeOperation = 0;
    int readOperation = 0;
    void purge();
    int timeToLive;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_TTLCACHESEGMENT_H_ */
