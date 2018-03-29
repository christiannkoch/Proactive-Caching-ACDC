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
/* @file EvictionStrategyFactory.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the EvictionStrategyFactory
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_EVICTIONSTRATEGYFACTORY_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_EVICTIONSTRATEGYFACTORY_H_
#include <string>
#include <vector>
#include "BasicEvictionStrategy.h"

/** @brief The CacheFactory class provides functionality for creating a cache
 *  TODO: change the implementation to a switch case statement, giving it only a Integer Value
 */
class EvictionStrategyFactory {
public:
    static BasicEvictionStrategy* createEvictionStrategy(std::string eviction,
            std::vector<std::string>* parameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy);
protected:
    EvictionStrategyFactory();
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_EVICTIONSTRATEGYFACTORY_H_ */
