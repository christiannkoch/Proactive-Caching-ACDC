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
/* @file OriginServer.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the OriginServer
 *
 * @section DESCRIPTION
 */
#ifndef SRC_SIMULATION_ORIGINSERVER_H_
#define SRC_SIMULATION_ORIGINSERVER_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "ReverseProxyCreator.h"
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "NetworkInformation.h"
#include "ReverseProxy.h"

using namespace omnetpp;

class OriginServer: public cSimpleModule {
protected:
    virtual void sendResponse(VideoSegment *pkg, int gateNumber);
    virtual void handleMessage(cMessage *rqst) override;
    virtual void initialize() override;
    // The finish() function is called by OMNeT++ at the end of the simulation:
    virtual void finish() override;
    ReverseProxyCreator* rpc;
    NetworkInformation* nwInfo;
    VideoSegment* generatePackage(SegmentRequest* rqst);
    void createReverseProxys();
    double serves = 0.0;
    double servesTotal = 0.0;
    cOutVector servesOverTime;
    void scheduleSelfMessages();
    void recordData();
    cMessage* recordingEvent;
    double timeDifference = 0.0;
    std::vector<cModule*> reverseProxys;
};

#endif /* SRC_SIMULATION_ORIGINSERVER_H_ */
