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
/* @file LargestGhostListSSAS.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the LargestGhostListSSAS
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_ACDC_LARGESTGHOSTLISTSSAS_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_ACDC_LARGESTGHOSTLISTSSAS_H_
#include "BasicSSAS.h"
#include "BasicGhostList.h"
#include "BasicCacheSegment.h"
#include "ARCProbationaryCache.h"
#include <vector>
class LargestGhostListSSAS : public BasicSSAS {
public:
    LargestGhostListSSAS(ARCProbationaryCache* probationaryCache,
            BasicGhostList* probationaryGhostList,
            std::vector<BasicCacheSegment*>* cacheSegmentVector,
            std::vector<BasicGhostList*>* ghostListVector, unsigned int cacheSize,
            double subCacheSize, int minSegSize);
    int getIdToReduce(unsigned int id);
    virtual ~LargestGhostListSSAS();
private:
    ARCProbationaryCache* probationaryCache;
    BasicGhostList* probationaryGhostList;
    std::vector<BasicCacheSegment*>* cacheSegmentVector;
    std::vector<BasicGhostList*>* ghostListVector;
    int cacheSize;
    int subCacheSize;
    int minSegSize;
    int getSmallestCacheSegment(int toExpand);
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_ACDC_LARGESTGHOSTLISTSSAS_H_ */
