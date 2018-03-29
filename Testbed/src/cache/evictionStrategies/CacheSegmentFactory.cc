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
/* @file CacheSegmentFactory.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements a factory for cacheSegments
 *
 * @section DESCRIPTION
 *
 * Returns the desired cache segment
 */
#include "CacheSegmentFactory.h"
#include <string>
#include "LFUCacheSegment.h"
#include "LRUCacheSegment.h"
#include "FIFOCacheSegment.h"
#include "RANDCacheSegment.h"
#include "TTLCacheSegment.h"

CacheSegmentFactory::CacheSegmentFactory() {
    // TODO Auto-generated constructor stub

}

/**
 * @brief crates a cache segment with a distinct eviction strategy
 *
 * @param type a string representing the eviction strategy of the cache segment
 * @param category the category for which this cache segment is responsible
 * @param size the size of the cache segment in kBit
 *
 * @return a pointer to a cache segment
 */
BasicCacheSegment* CacheSegmentFactory::createCacheSegment(std::string type,
        std::string category, long long size) {
    if (type == "LFU")
        return new LFUCacheSegment(size, category);
    else if (type == "LRU")
        return new LRUCacheSegment(size, category);
    else if (type == "RAND")
        return new RANDCacheSegment(size, category);
    else if (type == "TTL")
        return new TTLCacheSegment(size, category);
    else if (type == "FIFO")
        return new FIFOCacheSegment(size, category);
    else
        throw "This type of cache Segment does not exist";
}

