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
/* @file ARC1Cache.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the ARC1Cache
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_ARC1_ARC1CACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_ARC1_ARC1CACHE_H_

#include <map>
#include <string>
#include <vector>
#include "../BasicEvictionStrategy.h"
#include "../ghostLists/GhostList.h"
#include "../probationary/ARCProbationaryCache.h"
#include "../ProbationaryCacheFactory.h"
#include "../CacheSegmentFactory.h"
#include "../segment/LFUCacheSegment.h"
#include "../../../simulation/ReverseProxy.h"
/**
 * @brief The ARCCache class provides functionality to model the behaviour of an Adaptive Replacement Cache
 */
class ARC1Cache: public BasicEvictionStrategy {
public:
    /*
     * @brief Creates a new ARC Cache for caching functionalities
     * @param size The desired Size of this Cache
     * @param module The Module to which this cache belongs to
     * @return A pointer to this class
     */
    ARC1Cache(std::vector<std::string>* parameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy);
    virtual ~ARC1Cache();
    /**
     * @brief Prints the Content of the Cache
     */
    void periodicEvents();
    /**
     * @brief Inserts a Videopackage into the Cache
     * @param *pkg A Videopackage
     */
    void insertIntoCache(VideoSegment *pkg);
    /**
     * @brief Checks if the requested Package is already in the Cache
     * @param rqst a Videorequest
     * @return A bool Value, indicating wheteer the Cache contains the Package
     */
    bool contains(SegmentRequest *rqst);
    /**
     * @brief Retrieves the video segment from the cache. This should only be executed, if contains returns true.
     * @param rqst A Videorequest
     * @return The Videopackage that fullfills the Videorequest
     * @todo exceptionhandling to make sure, this can only be executed, if the object is in the cache.
     */
    VideoSegment *retrieveSegment(SegmentRequest *rqst);

    /**
     * @brief Get the size of the cache
     * @return Returns an integer Value describing the size of the cache
     * @todo Do I need this?
     */
    long long getSize();
    /**
     * @brief deletes all Objects in the Cache
     * @todo Maybe I should only perform this action in the destructor. Read up on this!
     */
    void clearCache();
    void resetRates();
    void deleteSegment(std::string id);
    int getWriteOperations();
    int getReadOperations();
    std::string getCountsOfElements();
protected:
    /**
     * @brief Sets the size of the Cache
     * @param size An Integer Value
     * @todo Check if this should really be public
     */
    void setSize(long long size);
    bool expanded;
    std::vector<std::string>* parameters;
    unsigned long long maxCacheSize;
    unsigned long long cacheSize;
    void rearrangeCache(VideoSegment *pkg); // wird in retrieveResponse aufgerufen
    long long subCacheSize;
    ARCProbationaryCache* t1;
    LFUCacheSegment* t2;
    GhostList* b1 = new GhostList();
    GhostList* b2 = new GhostList();
    std::string expandStrat;
    void alterCacheSize(double newCacheSize);
    std::vector<std::pair<double, double>>* storageAlterations;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_ARC1_ARC1CACHE_H_ */
