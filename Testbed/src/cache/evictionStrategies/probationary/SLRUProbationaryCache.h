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
/* @file SLRUProbationaryCache.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the SLRUProbationaryCache
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_SLRU_PROBATIONARYCACHE_
#define SRC_CACHE_SLRU_PROBATIONARYCACHE_
#include <map>
#include <string>
#include "BasicProbationaryCache.h"
#include "../nodes/RecencyNode.h"
#include "../../PointerAndCounter.h"

class SLRUProbationaryCache: public BasicProbationaryCache {
public:
    SLRUProbationaryCache(long long size);
    virtual ~SLRUProbationaryCache();
    void periodicEvents();
    std::list<std::string>* insertIntoCache(VideoSegment *pkg);
    bool contains(SegmentRequest *rqst);
    VideoSegment *retrieveSegment(SegmentRequest *rqst);
    void setSize(long long size);
    long long getSize();
    long long getMaxSize();
    void clearCache();
    void insertIntoCacheAtEnd(VideoSegment *pkg);
    int getReadOperations();
    int getWriteOperations();
    void drop(VideoSegment* pkg);
    void deleteSegment(std::string id);
    void resetRates();
    std::string getCountsOfElements();
protected:
    unsigned long long maxCacheSize;
    unsigned long long cacheSize = 0;
    void rearrangeCache(VideoSegment *pkg);
    RecencyNode* head;
    std::map<std::string, std::pair<PointerAndCounter*, RecencyNode*>*> container;
    int writeOperation = 0;
    int readOperation = 0;
};

#endif /* SRC_CACHE_SLRU_PROBATIONARYCACHE_ */
