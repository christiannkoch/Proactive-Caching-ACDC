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
/* @file SSASFactory.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the SSASFactory
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_ACDC_SSASFACTORY_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_ACDC_SSASFACTORY_H_
#include <string>
#include <vector>
#include "BasicSSAS.h"
#include "../ghostLists/BasicGhostList.h"
#include "../segment/BasicCacheSegment.h"
#include "../probationary/ARCProbationaryCache.h"
#include "BasicSSAS.h"
#include "LargestGhostListSSAS.h"
#include "SmallestGhostListSSAS.h"
#include "RelativeLargestGhostListSSAS.h"
#include "RelativeSmallestGhostListSSAS.h"

class SSASFactory {
public:
    SSASFactory();
    static BasicSSAS* createAdaptionStrategy(
            ARCProbationaryCache* probationaryCache,
            BasicGhostList* probationaryGhostList,
            std::vector<BasicCacheSegment*>* cacheSegmentVector,
            std::vector<BasicGhostList*>* ghostListVector, unsigned int cacheSize,
            int subCacheSize, double minSegSize, std::string type);
protected:
    ~SSASFactory();
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_ACDC_SSASFACTORY_H_ */
