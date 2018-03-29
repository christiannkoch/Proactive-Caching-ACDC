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
/* @file LFUDACache.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the LFUDACache
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_LFUDA_LFUDACACHE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_LFUDA_LFUDACACHE_H_

#include <nodes/FrequencyNode.h>
#include <map>
#include <string>
#include "ReverseProxy.h"
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "BasicEvictionStrategy.h"
class LFUDACache: public BasicEvictionStrategy {
public:
    LFUDACache(std::vector<std::string>* parameters, long long size,
            std::vector<std::pair<double, double>>* storageAlterations,
            std::string storageAlterationStrategy);
    virtual ~LFUDACache();
    void periodicEvents() override;
    void insertIntoCache(VideoSegment *pkg) override; // gibt die ID des Videos zurück, wenn es aus dem LFU gelöscht wird um sie dann in die zweite Ghostlist zu schreiben. Wenn die ID 0 ist, muss nichts in die GhostList geschreiben werden, da nichts gelöscht wurde.
    bool contains(SegmentRequest *rqst) override;
    VideoSegment *retrieveSegment(SegmentRequest *rqst) override;
    long long getSize() override;
    void clearCache() override;
    int getReadOperations();
    int getWriteOperations();
    void deleteSegment(std::string id);
    void resetRates();
protected:
    bool expanded;
    void setSize(long long size) override;
    void rearrangeCache(VideoSegment *pkg) override;
    std::string getLeastFrequent();
    std::map<std::string, std::pair<VideoSegment*, FrequencyNode*>*> container;
    std::map<int, FrequencyNode*> nodeContainer;
    FrequencyNode* head;
    unsigned long long maxCacheSize;
    unsigned long long cacheSize = 0;
    std::vector<std::string>* parameters;
    int writeOperation = 0;
    int readOperation = 0;
    int globalK;
    FrequencyNode* findPred(int key);
    FrequencyNode* findSucc(int key);
    std::string expandStrat;
    void alterCacheSize(double newCacheSize);
    std::vector<std::pair<double, double>>* storageAlterations;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_LFUDA_LFUDACACHE_H_ */
