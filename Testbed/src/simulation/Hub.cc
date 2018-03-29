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
/* @file Hub.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the Hub module
 *
 * @section DESCRIPTION
 */
#include "Hub.h"
#include <omnetpp.h>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "RequestHandler.h"
#include <vector>
#include <string>

Define_Module(Hub);

using namespace omnetpp;
/*
 * @brief function called at the beginning
 *
 * creates a requestHandler for the hub
 */
void Hub::initialize() {
    this->rqHandler = new RequestHandler();
}

/*
 * @brief defines the behaviour of the hub module when a message arrives
 *
 * @param msg the message that arrives at the hub
 */
void Hub::handleMessage(cMessage *msg) {
    const char* packetType = msg->getClassName();
    if (strcmp(packetType, "SegmentRequest") == 0) { // if it is a request
        SegmentRequest *rqst = check_and_cast<SegmentRequest *>(msg);
        std::string keyBuilder = rqst->getVideoId()
                + std::to_string(rqst->getSegmentId());
        rqHandler->insertRequest(keyBuilder,
                rqst->getArrivalGate()->getIndex()); // the requests videoId and arrival channel is stored in the requestHandler
        sendMessage(rqst, 0); // the request is forwareded
    } else { // else it is a segment
        VideoSegment* package = check_and_cast<VideoSegment *>(msg);
        std::string keyBuilder = package->getVideoId()
                + std::to_string(package->getSegmentId());
        std::vector<std::pair<int, double>*>* queue = rqHandler->getRequestList(
                keyBuilder); // get the queue for this videoSegment
        if (queue) { // if the queue exists
            for (std::vector<std::pair<int, double>*>::iterator iter =
                    queue->begin(); iter != queue->end(); iter++) // for every element in the queue
                {
                VideoSegment *copy = (VideoSegment *) package->dup();
                sendMessage(copy, (*iter)->first); //send a copy of the videoSegment to the client that requested it at some point in time
            }
            rqHandler->deleteList(keyBuilder); // then delete the queue
        }
        delete package;
    }
}

/*
 * @brief is called at the end of the simulation
 */
void Hub::finish() {
    delete rqHandler;
}

/*
 * @brief sends a message
 *
 * @param msg the message to send
 * @gate gate the gate on which to send the message
 */
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
