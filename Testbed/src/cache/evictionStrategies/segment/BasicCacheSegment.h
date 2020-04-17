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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_BASICCACHESEGMENT_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_BASICCACHESEGMENT_H_
#include <string>
#include "../../../simulation/SegmentRequest_m.h"
#include "../../../simulation/VideoSegment_m.h"
class BasicCacheSegment {
public:
    BasicCacheSegment();
    virtual ~BasicCacheSegment();
    virtual void setSize(long long size) = 0;
    virtual long long getSize() = 0;
    virtual long long getMaxSize() = 0;
    virtual void periodicEvents() = 0;
    virtual void clearCache() = 0;
    virtual bool contains(SegmentRequest *rqst) = 0;
    virtual std::list<std::string>* insertIntoCache(VideoSegment *pkg) = 0; // gibt die ID des Videos zurück, wenn es aus dem LFU gelöscht wird um sie dann in die zweite Ghostlist zu schreiben. Wenn die ID 0 ist, muss nichts in die GhostList geschreiben werden, da nichts gelöscht wurde.
    virtual VideoSegment* retrieveSegment(SegmentRequest *rqst) = 0;
    virtual void expand(int i) = 0;
    virtual std::list<std::string>* reduce(int i) = 0;
    virtual int getReadOperations() = 0;
    virtual int getWriteOperations() = 0;
    virtual std::string getCategory() = 0;
    virtual void deleteSegment(std::string id) = 0;
    virtual void resetRates() = 0;
    virtual std::string getCountsOfElements() = 0;
protected:
    virtual void rearrangeCache(VideoSegment *pkg) = 0;
    unsigned long long cacheSize = 0;
    virtual void setCategory(std::string category) = 0;
    int writeOperation = 0;
    int readOperation = 0;
    std::string category;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_BASICCACHESEGMENT_H_ */
