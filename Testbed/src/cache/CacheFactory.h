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
/* @file CacheFactory.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the CacheFactory
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_CACHEFACTORY_H_
#define SRC_CACHE_CACHEFACTORY_H_
#include <string>
#include <vector>
#include <omnetpp.h>
#include "../simulation/ReverseProxy.h"
#include "BaseCache.h"
#include "admissionStrategies/BasicAdmissionStrategy.h"
#include "evictionStrategies/BasicEvictionStrategy.h"
#include "admissionStrategies/AdmissionStrategyFactory.h"
#include "evictionStrategies/EvictionStrategyFactory.h"

class CacheFactory {
public:
    static BaseCache* createCache(std::string admissionImplementation,
            std::vector<std::string>* admissionParameters,
            std::string evictionImplementation,
            std::vector<std::string>* evictionParameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy, omnetpp::cModule* cModule);
protected:
    CacheFactory();
};

#endif /* SRC_CACHE_CACHEFACTORY_H_ */
