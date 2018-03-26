// This program is free software: you can redistribute it and/or modify
//
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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_BASICEVICTIONSTRATEGY_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_BASICEVICTIONSTRATEGY_H_

#include <vector>
#include <string>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

/** The Base Class for Caches
 *  This is an abstract Class where all Caches inherit from
 */
class BasicEvictionStrategy {
public:
    BasicEvictionStrategy();
    virtual ~BasicEvictionStrategy();
    virtual void periodicEvents() = 0;
    virtual void insertIntoCache(VideoSegment *pkg) = 0;
    virtual bool contains(SegmentRequest *rqst) = 0;
    virtual VideoSegment *retrieveSegment(SegmentRequest *rqst) = 0;
    virtual long long getSize() = 0;
    virtual void clearCache() = 0;
    virtual int getWriteOperations() = 0;
    virtual int getReadOperations() = 0;
    virtual void deleteSegment(std::string id) = 0;
    virtual void resetRates() = 0;
protected:
    virtual void setSize(long long size) = 0;
    std::vector<std::string>* parameters;
    std::vector<std::pair<double, double>>* storageAlterations;
    long long cacheSize = 0;
    virtual void rearrangeCache(VideoSegment *pkg) = 0;
    std::string expandStrat;
    virtual void alterCacheSize(double newCacheSize) = 0;

};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_BASICEVICTIONSTRATEGY_H_ */
