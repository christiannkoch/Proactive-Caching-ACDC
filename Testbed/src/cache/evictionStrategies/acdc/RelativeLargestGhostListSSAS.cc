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
/* @file RelativeLargestGhostListSSAS.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the RelativeLargestGhostListSSAS
 *
 * @section DESCRIPTION
 */
#include "RelativeLargestGhostListSSAS.h"
/*
 * @brief returns an instance of a RelativeLargestGhostListSSAS
 * @param probationaryCache the probationary cache
 * @param probationaryGhostList the probationary ghostlist
 * @param cacheSegmentVector the category cache vector
 * @param ghostListVector the ghostlists for the category caches
 * @param cacheSize the size of the cache
 * @param subCacheSize the subcache size
 * @param minSegSize the minimum subcache size
 * @return an instance of a RelativeLargestGhostListSSAS
 */
RelativeLargestGhostListSSAS::RelativeLargestGhostListSSAS(
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
int RelativeLargestGhostListSSAS::getIdToReduce(unsigned int toExpand) {
    int idToReduce = 0;
    double relativeGhostListSize = 0;
    double cacheSizesToCompare = 0;
    int sumOfGhostListEntries = 1;
    if (getSumOfGhostListEntries() > 0)
        sumOfGhostListEntries = getSumOfGhostListEntries();
    if (toExpand != 0) {
        if (probationaryCache->getSize() > (subCacheSize / minSegSize)) {
            idToReduce = 0;
            relativeGhostListSize = (double) (probationaryGhostList->getSize()
                    / (double) sumOfGhostListEntries);
            cacheSizesToCompare = probationaryCache->getSize();
        }
    } else {
        idToReduce = getSmallestCacheSegment(toExpand);

        relativeGhostListSize =
                (double) (ghostListVector->at(idToReduce - 1)->getSize()
                        / (double) sumOfGhostListEntries);
        cacheSizesToCompare = cacheSegmentVector->at(idToReduce - 1)->getSize();
    }
    for (unsigned int i = 1; i <= ghostListVector->size(); i++) {
        if (i != toExpand) {
            if (cacheSegmentVector->at(i - 1)->getSize()
                    > ((double) subCacheSize / (double) minSegSize)) {
                double ghostListDelta = (double) relativeGhostListSize
                        / (double) (ghostListVector->at(i - 1)->getSize()
                                / (double) sumOfGhostListEntries);
                double cacheSizeDelta = (double) cacheSizesToCompare
                        / (double) cacheSegmentVector->at(i - 1)->getSize();
                if (ghostListDelta >= cacheSizeDelta) {
                    idToReduce = i;
                    relativeGhostListSize =
                            (double) (ghostListVector->at(i - 1)->getSize()
                                    / (double) sumOfGhostListEntries);
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
int RelativeLargestGhostListSSAS::getSumOfGhostListEntries() {
    int sumOfGhostListEntries = probationaryGhostList->getSize();
    for (unsigned int i = 0; i < ghostListVector->size(); i++) {
        sumOfGhostListEntries = sumOfGhostListEntries
                + ghostListVector->at(i)->getSize();
    }
    return sumOfGhostListEntries;
}
/*
 * @brief returns the smallest cache segment
 * @param toExpand the cache segment to expand
 * @return the smallest cache segment
 */
int RelativeLargestGhostListSSAS::getSmallestCacheSegment(int toExpand) {
    int smallestCache = 0;
    int smallestCacheSize = 0;
    if (toExpand != 0) {
        smallestCache = 0;
        smallestCacheSize = probationaryCache->getSize();
    }

    else {
        smallestCache = 1;
        smallestCacheSize = cacheSegmentVector->at(0)->getSize();
    }
    for (unsigned int i = 0; i < cacheSegmentVector->size(); i++) {
        if (smallestCacheSize >= cacheSegmentVector->at(i)->getSize()) {
            smallestCacheSize = cacheSegmentVector->at(i)->getSize();
            smallestCache = i + 1;
        }
    }
    return smallestCache;
}

RelativeLargestGhostListSSAS::~RelativeLargestGhostListSSAS() {
}

/*
 * @brief sets the new SubCacheSize
 *
 * a new subcache size needs to be set after a cache size alteration happened
 * to ensure that the SSAS works properly
 * @param newSubcacheSize the new size of the subcaches
 *
 */
void RelativeLargestGhostListSSAS::setNewSubcacheSize(
        long long newSubcacheSize) {
    this->subCacheSize = newSubcacheSize;
}
