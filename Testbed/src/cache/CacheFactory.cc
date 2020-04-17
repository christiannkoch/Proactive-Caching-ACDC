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
/* @file CacheFactory.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief a factory for the desired Base Cache
 *
 * @section DESCRIPTION
 *
 * This is a factory class for the aviable caching strategies
 *
 */


#include "CacheFactory.h"
#include <string>
#include <vector>

CacheFactory::CacheFactory() {

}

/**
 * @brief creates a type of cache based on the input
 *
 * The method creates an eviction and an admission strategy combines these in the baseCache Class
 *
 * @param admission a string that defines the type of admission strategy
 * @param admissionParameters several parameters that may be needed for the specific admission strategy
 * @param eviction a string that defines the type of eviction strategy
 * @param evictionParameters serveral parameters that may be needed for the specific admission strategy
 * @param size an integer representing the size of the cache in kBit
 * @param storageAlterations an optional vector with any number of pairs of doubles. The first representing a
 * time the second one representing a cache size in kBit.
 * @param storageAlterationStrategy the alteration strategy that defines the behaviour of the storage alteration
 * @param cModule a pointer to the module that called this function (may be obsolete?)
 *
 *
 * @return a pointer to a BaseCache
 *
 */

BaseCache* CacheFactory::createCache(std::string admission,
        std::vector<std::string>* admissionParameters, std::string eviction,
        std::vector<std::string>* evictionParameters, long long size,
        std::vector<std::pair<double, double>>* storageAlterations,
        std::string storageAlterationStrategy, omnetpp::cModule* cModule) {
    BasicAdmissionStrategy* admissionStrategy;
    try {
        admissionStrategy = AdmissionStrategyFactory::createAdmissionStrategy(
                admission, admissionParameters);
    } catch (char const* msg)

    {
        std::cout << msg;
    }
    BasicEvictionStrategy * evictionStrategy;
    try {
        evictionStrategy = EvictionStrategyFactory::createEvictionStrategy(
                eviction, evictionParameters, size, storageAlterations,
                storageAlterationStrategy);
    } catch (char const* msg)

    {
        std::cout << msg;
    }
    return new BaseCache(admissionStrategy, evictionStrategy, cModule);
}

