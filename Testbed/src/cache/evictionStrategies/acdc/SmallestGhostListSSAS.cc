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

#include "SmallestGhostListSSAS.h"
/*
 * @brief returns an instance of a SmallestGhostListSSAS
 * @param probationaryCache the probationary cache
 * @param probationaryGhostList the probationary ghostlist
 * @param cacheSegmentVector the category cache vector
 * @param ghostListVector the ghostlists for the category caches
 * @param cacheSize the size of the cache
 * @param subCacheSize the subcache size
 * @param minSegSize the minimum subcache size
 * @return an instance of a SmallestGhostListSSAS
 */
SmallestGhostListSSAS::SmallestGhostListSSAS(
        ARCProbationaryCache* probationaryCache,
        BasicGhostList* probationaryGhostList,
        std::vector<BasicCacheSegment*>* cacheSegmentVector,
        std::vector<BasicGhostList*>* ghostListVector, unsigned int cacheSize,
        double subCacheSize, int minSegSize) {
    this->probationaryCache = probationaryCache;
    this->probationaryGhostList = probationaryGhostList;
    this->cacheSegmentVector = cacheSegmentVector;
    this->ghostListVector = ghostListVector;
    this->cacheSize = cacheSize;
    this->subCacheSize = subCacheSize;
    this->minSegSize = minSegSize;

}
/*
 * @brief returns the id of the subcache that is reduced
 * @param toExpand the id of the subcahe that is expanded
 *
 * decides which of subcache is reduced based upon the size of the ghostlists
 *
 * @return the id of the subcache that is reduced. 0 means the probationary cache
 */
int SmallestGhostListSSAS::getIdToReduce(unsigned int toExpand) {
    int idToReduce = 0;
    double ghostListSize = 0;
    if (toExpand != 0) {
        if (probationaryCache->getSize() > (subCacheSize / minSegSize)) {
            ghostListSize = probationaryGhostList->getSize();
            idToReduce = 0;
        }
    } else {
        ghostListSize = ghostListVector->at(idToReduce)->getSize();
        idToReduce = getLargestCacheSegment(toExpand);

    }
    for (unsigned int i = 1; i <= ghostListVector->size(); i++) {
        if (i != toExpand) {
            if (cacheSegmentVector->at(i - 1)->getSize()
                    > (subCacheSize / minSegSize))
                if (ghostListVector->at(i - 1)->getSize() < ghostListSize) {
                    idToReduce = i;
                    ghostListSize = ghostListVector->at(i - 1)->getSize();
                }
        }
    }
    return idToReduce;

}

SmallestGhostListSSAS::~SmallestGhostListSSAS() {
}
/*
 * @brief returns the largest cache segment
 * @param toExpand the cache segment to expand
 * @return the largest cache segment
 */
int SmallestGhostListSSAS::getLargestCacheSegment(int toExpand) {
    int largestCache = 0;
    int largestCacheSize = 0;
    if (toExpand != 0) {
        largestCache = 0;
        largestCacheSize = probationaryCache->getSize();
    }

    else {
        largestCache = 1;
        largestCacheSize = cacheSegmentVector->at(0)->getSize();
    }
    for (unsigned int i = 0; i < cacheSegmentVector->size(); i++) {
        if (largestCacheSize <= cacheSegmentVector->at(i)->getSize()) {
            largestCacheSize = cacheSegmentVector->at(i)->getSize();
            largestCache = i + 1;
        }
    }
    return largestCache;
}
