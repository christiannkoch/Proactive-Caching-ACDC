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
/* @file SSASFactory.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief returns the desired SSAS
 *
 * @section DESCRIPTION
 */
#include "SSASFactory.h"
#include "BasicSSAS.h"
#include "LargestGhostListSSAS.h"
#include "SmallestGhostListSSAS.h"
#include "RelativeLargestGhostListSSAS.h"
#include "RelativeSmallestGhostListSSAS.h"
/*
 * @brief returns an instance of the SSASFactory
 * @return an instance of the SSASFactory
 */
SSASFactory::SSASFactory() {

}
/*
 * @brief returns an adpative Strategy based upon the type we want
 * @param probationaryCache the probationary cache
 * @param probationaryGhostList the probationary ghostlist
 * @param cacheSegmentVector the category cache vector
 * @param ghostListVector the ghostlists for the category caches
 * @param cacheSize the size of the cache
 * @param subCacheSize the subcache size
 * @param minSegSize the minimum subcache size
 * @param type the type of the adaption strategy we want
 * @return an adaptive Strategy
 */
BasicSSAS* SSASFactory:: createAdaptionStrategy(
        ARCProbationaryCache* probationaryCache,
        BasicGhostList* probationaryGhostList,
        std::vector<BasicCacheSegment*>* cacheSegmentVector,
        std::vector<BasicGhostList*>* ghostListVector, unsigned int cacheSize,
        int subCacheSize, double minSegSize, std::string type) {
    BasicSSAS* returnedSSAS = nullptr;
    if (type == "largestGhostList") {
        returnedSSAS = new LargestGhostListSSAS(probationaryCache,
                probationaryGhostList, cacheSegmentVector, ghostListVector,
                cacheSize, subCacheSize, minSegSize);
    }
    else if (type == "smallesGhostList") {
        returnedSSAS = new SmallestGhostListSSAS(probationaryCache,
                probationaryGhostList, cacheSegmentVector, ghostListVector,
                cacheSize, subCacheSize, minSegSize);
    }
    else if (type == "relativeLargestGhostList") {
        returnedSSAS = new RelativeLargestGhostListSSAS(probationaryCache,
                probationaryGhostList, cacheSegmentVector, ghostListVector,
                cacheSize, subCacheSize, minSegSize);
    }
    else if (type == "relativeSmallestGhostList") {
        returnedSSAS = new RelativeSmallestGhostListSSAS(probationaryCache,
                probationaryGhostList, cacheSegmentVector, ghostListVector,
                cacheSize, subCacheSize, minSegSize);
    }
    else
        throw "This type of adaption Strategy does not exist\n";
    return returnedSSAS;
}
