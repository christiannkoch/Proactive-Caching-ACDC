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
/* @file BaseCache.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the BaseCache
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_BASECACHE_H_
#define SRC_CACHE_BASECACHE_H_
#include <omnetpp.h>

#include "admissionStrategies/BasicAdmissionStrategy.h"
#include "evictionStrategies/BasicEvictionStrategy.h"

class BaseCache {
public:
    BaseCache(BasicAdmissionStrategy* admission, BasicEvictionStrategy* eviction, omnetpp::cModule* cModule);
    virtual ~BaseCache();
    BasicAdmissionStrategy* getAdmissionStrategy();
    BasicEvictionStrategy* getEvictionStrategy();
    int getWriteOperations();
    int getReadOperations();
private:
    BasicAdmissionStrategy* admission;
    BasicEvictionStrategy* eviction;
    omnetpp::cModule* proxy;
};

#endif /* SRC_CACHE_BASECACHE_H_ */
