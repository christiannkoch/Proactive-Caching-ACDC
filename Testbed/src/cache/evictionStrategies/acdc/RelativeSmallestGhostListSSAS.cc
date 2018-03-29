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
/* @file RelativeSmallestGhostListSSAS.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the RelativeSmallestGhostListSSAS
 *
 * @section DESCRIPTION
 */
#include "RelativeSmallestGhostListSSAS.h"
/*
 * @brief returns an instance of a RelativeSmallestGhostListSSAS
 * @param probationaryCache the probationary cache
 * @param probationaryGhostList the probationary ghostlist
 * @param cacheSegmentVector the category cache vector
 * @param ghostListVector the ghostlists for the category caches
 * @param cacheSize the size of the cache
 * @param subCacheSize the subcache size
 * @param minSegSize the minimum subcache size
 * @return an instance of a RelativeSmallestGhostListSSAS
 */
RelativeSmallestGhostListSSAS::RelativeSmallestGhostListSSAS(
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
 * decides which of subcache is reduced based upon the size of the subcache relative to its ghostlist
 *
 * @return the id of the subcache that is reduced. 0 means the probationary cache
 */
int RelativeSmallestGhostListSSAS::getIdToReduce(unsigned int toExpand) {
    int idToReduce = 0;
    double relativeGhostListSize = 0;
    double cacheSizesToCompare = 0;
    int sumOfGhostListEntries = 1;
    if (getSumOfGhostListEntries() > 0)
        sumOfGhostListEntries = getSumOfGhostListEntries();
    if (toExpand != 0) {
        if (probationaryCache->getSize() > (subCacheSize / minSegSize)) {
            idToReduce = 0;
            relativeGhostListSize = (probationaryGhostList->getSize()
                    / sumOfGhostListEntries);
            cacheSizesToCompare = probationaryCache->getSize();
        }
    } else {
        idToReduce = getLargestCacheSegment(toExpand);

        relativeGhostListSize = (ghostListVector->at(idToReduce - 1)->getSize()
                / sumOfGhostListEntries);
        cacheSizesToCompare = cacheSegmentVector->at(idToReduce - 1)->getSize();
    }

    for (unsigned int i = 1; i <= ghostListVector->size(); i++) {
        if (i != toExpand) {
            if (cacheSegmentVector->at(i - 1)->getSize()
                    > (subCacheSize / minSegSize)) {
                double ghostListDelta = relativeGhostListSize
                        / (ghostListVector->at(i - 1)->getSize()
                                / sumOfGhostListEntries);
                double cacheSizeDelta = cacheSizesToCompare
                        / cacheSegmentVector->at(i - 1)->getSize();
                if (ghostListDelta <= cacheSizeDelta) {
                    idToReduce = i;
                    relativeGhostListSize =
                            (ghostListVector->at(i - 1)->getSize()
                                    / sumOfGhostListEntries);
                    cacheSizesToCompare =
                            cacheSegmentVector->at(i - 1)->getSize();
                }
            }

        }
    }
    return idToReduce;
}
/*
 * @brief calculates the sum of all ghostlist entries
 * @return the sum of all ghostlist entries
 */
int RelativeSmallestGhostListSSAS::getSumOfGhostListEntries() {
    int sumOfGhostListEntries = probationaryGhostList->getSize();
    for (unsigned int i = 0; i < ghostListVector->size(); i++) {
        sumOfGhostListEntries = sumOfGhostListEntries
                + ghostListVector->at(i)->getSize();
    }
    return sumOfGhostListEntries;
}
/*
 * @brief returns the largest cache segment
 * @param toExpand the cache segment to expand
 * @return the largest cache segment
 */
int RelativeSmallestGhostListSSAS::getLargestCacheSegment(int toExpand) {
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

RelativeSmallestGhostListSSAS::~RelativeSmallestGhostListSSAS() {
    // TODO Auto-generated destructor stub
}
