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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_RANDCACHESEGMENT_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_RANDCACHESEGMENT_H_
#include <string>
#include <map>
#include "../../../simulation/SegmentRequest_m.h"
#include "../../../simulation/VideoSegment_m.h"
#include "BasicCacheSegment.h"
#include "../../PointerAndCounter.h"

class RANDCacheSegment: public BasicCacheSegment {
public:
    RANDCacheSegment(long long size, std::string category);
    virtual ~RANDCacheSegment();
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
    std::string getCountsOfElements();
protected:
    unsigned long long maxCacheSize;
        unsigned long long cacheSize;
    void rearrangeCache(VideoSegment* pkg);
    void setCategory(std::string category);
    std::map<std::string, PointerAndCounter*> container;
    std::string category;
    int writeOperation = 0;
    int readOperation = 0;
    std::vector<std::string> keyList;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_RANDCACHESEGMENT_H_ */
