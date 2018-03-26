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

#include "OriginServer.h"
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <ReverseProxyCreator.h>
#include "NetworkInformation.h"
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"

Define_Module(OriginServer);

/*
 * @brief called when this module is created
 */
void OriginServer::initialize() {
    std::cout << "Initialisierung startet \n"; //used for debugging purposes
    this->nwInfo = NetworkInformation::getInformation(this); //creates the NetworkInformation
    nwInfo->setupNetworkInformation();
    setGateSize("originGateIn", nwInfo->getGateSize(0));
    setGateSize("originGateOut", nwInfo->getGateSize(0));
    createReverseProxys();
    recordingEvent = new cMessage("next Recording");
    scheduleAt(0.0, recordingEvent);
    timeDifference = getParentModule()->par("recordGranularity");
    scheduleSelfMessages();
    servesOverTime.setName("Serves");
}

/*
 * @brief schedules all self messages for recording purposes
 */
void OriginServer::scheduleSelfMessages() {
    for (double i = 0; i <= (nwInfo->getSimulationDuration() / timeDifference);
            i = i + 1) {
        scheduleAt(i * timeDifference, recordingEvent->dup());
    }
}

/*
 * @brief records the metrics from the origin server
 */
void OriginServer::recordData() {
    servesOverTime.record(serves);
    serves = 0.0;
}

/*
 * @brief describes the behaviour when a message arrives
 *
 * on arrival of a next recording message, metrics are recorded
 * on the arrival of a segment request, this request is served
 */
void OriginServer::handleMessage(cMessage *msg) {
    if (strcmp("next Recording", msg->getName()) == 0) {
        recordData();
        delete msg;
    } else {
        const char* packetType = msg->getClassName();
        if (strcmp(packetType, "SegmentRequest") == 0) {
            SegmentRequest *rqst = check_and_cast<SegmentRequest *>(msg);
            sendResponse(generatePackage(rqst),
                    rqst->getArrivalGate()->getIndex());
            delete rqst;
        } else {
            //VideoPackage *package = check_and_cast<VideoPackage *>(msg);
        }
    }
}

/*
 * @brief generates a video segment and sends
 * @param rqst the request that is to be fullfilled
 * @return a video segment that answers the request
 */
VideoSegment* OriginServer::generatePackage(SegmentRequest* rqst) {
    char pkgname[20];
    sprintf(pkgname, "Package id %s%s", rqst->getVideoId(),
            std::to_string(rqst->getSegmentId()).c_str());
    VideoSegment *pkg = new VideoSegment(pkgname);
    pkg->setVideoId(rqst->getVideoId());
    pkg->setSegmentId(rqst->getSegmentId());
    pkg->setClientId(rqst->getClientId());
    pkg->setUploader(rqst->getUploader());
    pkg->setCategory(rqst->getCategory());
    pkg->setSize(rqst->getSize());
    pkg->setSeenAbove(true);
    return pkg;
}

/*
 * @brief sends the video segment on the channel where the request came from
 * @param pkg the video segment to send
 * @param gateNumber the out gate on which to send the video segment
 */
void OriginServer::sendResponse(VideoSegment *pkg, int gateNumber) {
    send(pkg, "originGateOut", gateNumber);
    //bubble("Send Package");
    serves++;
    servesTotal++;
}

/*
 * @brief creates the reverse proxies
 */
void OriginServer::createReverseProxys() {
    rpc = new ReverseProxyCreator(getParentModule(), this,
            nwInfo->getAmountOfClients(), nwInfo->getAmountOfLeafProxys(),
            nwInfo->getAmountOfLevels(), nwInfo->getCDNDelay(),
            nwInfo->getProxyCacheSettings(), nwInfo->getLeafProxyVector(),
            nwInfo->getConnectionTable());
    rpc->createReverseProxys();
    delete rpc;
}

/*
 * @brief called when se simulation is over
 */
void OriginServer::finish() {
    servesOverTime.record(serves);
    recordScalar("Total serves", servesTotal);
    delete nwInfo;
}
