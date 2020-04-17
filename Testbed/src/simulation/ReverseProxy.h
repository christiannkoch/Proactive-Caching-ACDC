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
/* @file ReverseProxy.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the ReverseProxy
 *
 * @section DESCRIPTION
 */
#ifndef SRC_SIMULATION_REVERSEPROXY_H_
#define SRC_SIMULATION_REVERSEPROXY_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include "../cache/ProxyCacheSettingst.h"
#include "BaseCache.h"
#include "BasicEvictionStrategy.h"
#include "BasicAdmissionStrategy.h"
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "ClientCreator.h"
#include "NetworkInformation.h"
#include "RequestHandler.h"

using namespace omnetpp;

class ReverseProxy: public cSimpleModule {
public:
    std::string getCountsOfElements();
protected:
    void sendMessage(cMessage *msg, int gate);
    void handleMessage(cMessage *msg) override;
    void initialize() override;
    void finish() override;
    ProxyCacheSettings_t* settings;
    int cacheSize = 0;
    BasicEvictionStrategy* eviction;
    BasicAdmissionStrategy* admission;
    BaseCache* cache;
    void createClients();
    NetworkInformation* nwInfo;
    double timeDifference = 0.0;
    time_t lastRecorded = 0;
    cOutVector hits;
    cOutVector misses;
    cOutVector readOperations;
    cOutVector writeOperations;
    cOutVector hitRate;
    cOutVector avgDelay;
    cOutVector timeToFirstSegment;
    void recordData();
    void registerCacheHit();
    void registerCacheMiss();
    double cacheHit = 0.0;
    double cacheMiss = 0.0;
    double cacheHitTotal = 0.0;
    double cacheMissTotal = 0.0;
    RequestHandler* rqHandler;
    void scheduleSelfMessages();
    std::vector<double> delayVector;
    std::vector<double> firstSegmentDelayVector;
    std::string elementCounterAtEnd;
};

#endif /* SRC_SIMULATION_REVERSEPROXY_H_ */
