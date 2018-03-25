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

#ifndef SRC_CACHE_PROXYCACHSESETTINGST_H_
#define SRC_CACHE_PROXYCACHSESETTINGST_H_
#include <string>
#include <vector>

/**
 * @brief cache settings
 *
 * A struct that holds all information for a proxy cache. It consists of the admission strategy,
 * the eviction strategy, the size of the cache, the parameters for the eviction strategy,
 * the parameters for the admission strategy, the name of the proxy to use in the evaulation,
 * a vector of a pair of two double values, which can represent storage alterations,
 * the strategy used for the storage alteration and a boolean value indicating, if this proxy is
 * a leaf proxy
 */
struct ProxyCacheSettings_t {

    std::string admissionStrategy;
    std::string evictionStrategy;
    long long size;
    std::vector<std::string> evictionParameters;
    std::vector<std::string> admissionParameters;
    std::string name;
    std::vector<std::pair<double, double>>* storageAlterations;
    std::string storageAlterationStrategy;
    bool leaf;
    ProxyCacheSettings_t(std::string admissionStrategy,
            std::string evictionStrategy, long long size,
            std::vector<std::string> evictionParameters,
            std::vector<std::string> admissionParameters, std::string name,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy, bool leaf) :
            admissionStrategy(admissionStrategy), evictionStrategy(
                    evictionStrategy), size(size), evictionParameters(
                    evictionParameters), admissionParameters(
                    admissionParameters), name(name), storageAlterations(
                    storageAlterations), storageAlterationStrategy(
                    storageAlterationStrategy), leaf(leaf) {
    }

};

#endif /* SRC_CACHE_PROXYCACHSESETTINGST_H_ */
