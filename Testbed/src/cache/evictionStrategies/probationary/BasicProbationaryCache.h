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
/* @file BasicProbationaryCache.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the BasicProbationaryCache
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_PROBATIONARY_BASICPROBATIONARYCACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_PROBATIONARY_BASICPROBATIONARYCACHE_H_
#include "../../../simulation/SegmentRequest_m.h"
#include "../../../simulation/VideoSegment_m.h"
#include <string>

class BasicProbationaryCache {
public:
    BasicProbationaryCache();
    virtual ~BasicProbationaryCache();
    virtual void periodicEvents() = 0;
    virtual std::list<std::string>* insertIntoCache(VideoSegment *pkg) = 0;
    virtual bool contains(SegmentRequest *rqst) = 0;
    virtual VideoSegment* retrieveSegment(SegmentRequest *rqst) = 0;
    virtual void setSize(long long size) = 0;
    virtual long long getSize() = 0;
    virtual long long getMaxSize() = 0;
    virtual void clearCache() = 0;
    virtual int getReadOperations() = 0;
    virtual int getWriteOperations() = 0;
    virtual void deleteSegment(std::string id) = 0;
    virtual void resetRates() = 0;
    std::string getCountsOfElements();
protected:
    unsigned long long maxCacheSize = 0;
    unsigned long long cacheSize = 0;
    void rearrangeCache(VideoSegment *pkg);
    int writeOperation = 0;
    int readOperation = 0;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_PROBATIONARYBASECACHE_H_ */
