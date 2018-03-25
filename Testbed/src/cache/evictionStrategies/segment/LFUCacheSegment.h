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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_LFUCACHESEGMENT_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_LFUCACHESEGMENT_H_
#include <nodes/FrequencyNode.h>
#include <string>
#include <map>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "BasicCacheSegment.h"

class LFUCacheSegment: public BasicCacheSegment {
public:
    LFUCacheSegment(long long size, std::string category);
    virtual ~LFUCacheSegment();
    void setSize(long long size);
    long long getSize();
    long long getMaxSize();
    void periodicEvents();
    void clearCache();
    bool contains(SegmentRequest* rqst);
    std::list<std::string>* insertIntoCache(VideoSegment* pkg); // gibt die ID des Videos zurück, wenn es aus dem LFU gelöscht wird um sie dann in die zweite Ghostlist zu schreiben. Wenn die ID 0 ist, muss nichts in die GhostList geschreiben werden, da nichts gelöscht wurde.
    VideoSegment* retrievePackage(SegmentRequest* rqst);
    void expand(int i);
    std::list<std::string>* reduce(int i);
    int getReadOperations();
    int getWriteOperations();
    std::string getCategory();
    void deletePackage(std::string id);
    void resetRates();
protected:
    std::string getLeastFrequent();
    void rearrangeCache(VideoSegment* pkg);
    void setCategory(std::string category);
    std::map<std::string, std::pair<VideoSegment*, FrequencyNode*>*> container;

    FrequencyNode* head;
    std::string category;
    unsigned long long maxCacheSize;
        unsigned long long cacheSize = 0;
    int writeOperation = 0;
    int readOperation = 0;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_SEGMENT_LFUCACHESEGMENT_H_ */
