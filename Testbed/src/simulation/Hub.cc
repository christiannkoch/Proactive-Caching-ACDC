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

#include "Hub.h"
#include <omnetpp.h>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "RequestHandler.h"
#include <vector>
#include <string>

Define_Module(Hub);

using namespace omnetpp;

void Hub::initialize() {
    this->rqHandler = new RequestHandler();
}

void Hub::handleMessage(cMessage *msg) {
    const char* packetType = msg->getClassName();
    if (strcmp(packetType, "SegmentRequest") == 0) {
        SegmentRequest *rqst = check_and_cast<SegmentRequest *>(msg);
        std::string keyBuilder = rqst->getVideoId()
                + std::to_string(rqst->getSegmentId());
        rqHandler->insertRequest(keyBuilder,
                rqst->getArrivalGate()->getIndex());
        sendMessage(rqst, 0);
    } else {
        VideoSegment* package = check_and_cast<VideoSegment *>(msg);
        std::string keyBuilder = package->getVideoId()
                + std::to_string(package->getSegmentId());
        std::vector<std::pair<int, double>*>* queue = rqHandler->getRequestList(
                keyBuilder);
        if (queue) {
            for (std::vector<std::pair<int, double>*>::iterator iter =
                    queue->begin(); iter != queue->end(); iter++) {
                VideoSegment *copy = (VideoSegment *) package->dup();
                sendMessage(copy, (*iter)->first);
            }
            rqHandler->deleteList(keyBuilder);
        }
        delete package;
    }
}

void Hub::finish() {
    delete rqHandler;
}

void Hub::sendMessage(cMessage *msg, int gate) {
    const char* packetType = msg->getClassName();
    if (strcmp(packetType, "SegmentRequest") == 0) {
        SegmentRequest *rqst = check_and_cast<SegmentRequest *>(msg);
        EV << "Sending Request " << rqst << " to Origin. \n";
        send(rqst, "proxyOut");
    } else {
        VideoSegment *package = check_and_cast<VideoSegment *>(msg);
        send(package, "clientOut", gate);
    }
}
