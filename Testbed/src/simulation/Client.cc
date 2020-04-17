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
/* @file Client.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the client module
 *
 * @section DESCRIPTION
 */
#include "Client.h"
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>


Define_Module(Client);

using namespace omnetpp;

/*
 * @brief initializes the client module
 *
 * when a client is created he calls initialize on himself. the client
 * sets the userId and the clientId. It then schedules its first segment
 * request
 */
void Client::initialize() {
    this->nwInfo = NetworkInformation::getInformation(this);
    this->userId = par("userId");
    this->clientId = par("clientId");
    getRequests();
    /*
     * if the clients request set is not empty, take the first request, segment it
     * and schedule the first segment. a self message is scheduled every five seconds
     * which checks if there are still segment requests left.
     */
    if (!requestSet->empty()) {
        result = requestSet->at(requestSet->size() - 1);
        requestSet->pop_back();
        nextRequestEvent = new cMessage("next Request");
        periodicRequest = new cMessage("periodic Request");
        videoDuration = std::atof((result->at(4).c_str()));
        chunkDuration = 5.0;

        if (videoDuration > std::atof((result->at(7).c_str())))
            videoDuration = std::atof((result->at(7).c_str()));
        numberOfRequests = videoDuration / chunkDuration;
        if (numberOfRequests < 1)
            numberOfRequests = 1;
        if ((simTime()
                <= std::atof(result->at(1).c_str())
                        - nwInfo->getFirstRequestTimed()))
            scheduleAt(
                    (std::atof(result->at(1).c_str())
                            - nwInfo->getFirstRequestTimed()),
                    nextRequestEvent);
    }
}

/*
 * @brief the behaviour that occurs when a message arrives
 *
 * @param msg the message that arrives at the client gate
 *
 * in this method, the clients behaviour is described when a message arrives.
 */
void Client::handleMessage(cMessage *msg) {

    /**
     * if the type is nextRequestEvent, it is signaled that the client starts watching a new
     * video which results in new segmentRequests and the counter is reset.
     */
    if (msg == nextRequestEvent) {
        counter = 1;
        sendRequest(generateRequest(result, counter));
        counter++;
        scheduleAt(simTime().dbl() + (chunkDuration - 1), periodicRequest);
    }else {
        /*
         * the message type periodic request signals that the client is not done watching the
         * video and schedules a new segmentRequest
         */
        if (msg == periodicRequest) {
            if (numberOfRequests > counter) { // if tehre are still segments to be requested
                sendRequest(generateRequest(result, counter));
                counter++;
                scheduleAt(simTime().dbl() + (chunkDuration - 1), msg);
            }
           // otherwise we are done watching the current video.
            else {
                delete result;
                /*
                 * if the request set is not empty we take the next video and schedule segmentRequests for it
                 */
                if (!requestSet->empty()) {
                    result = requestSet->at(requestSet->size() - 1);
                    requestSet->pop_back();
                    videoDuration = std::atof((result->at(4).c_str()));
                    if (videoDuration > std::atof((result->at(7).c_str())))
                        videoDuration = std::atof((result->at(7).c_str()));
                    numberOfRequests = videoDuration / chunkDuration;
                    if (numberOfRequests < 1)
                        numberOfRequests = 1;

                    if (std::atof(result->at(1).c_str())
                            - nwInfo->getFirstRequestTimed()
                            > simTime().dbl()) {
                        counter = 0;
                        scheduleAt(
                                (std::atof(result->at(1).c_str())
                                        - nwInfo->getFirstRequestTimed()),
                                periodicRequest);
                    } else {
                        counter = 1;
                        counter++;
                        sendRequest(generateRequest(result, counter));
                        scheduleAt(simTime().dbl() + (chunkDuration - 1),
                                periodicRequest);
                    }

                }
                /*
                 * if the request set is empty, this client is done and does not send any more requests
                 */
                else {
                    delete requestSet;
                    delete msg;
                }
            }

        } else {
            delete msg;
        }
    }

}
/*
 * @brief generates a new segment request
 *
 * @param data the data of the video we generate requests for
 * @param i the segment we request
 *
 * generates a segmentRequest for a video
 */
SegmentRequest *Client::generateRequest(std::vector<std::string>* data, int i) {
    char rqstname[20];
    sprintf(rqstname, "Request id %s%s", data->at(3).c_str(),
            std::to_string(i).c_str());
    SegmentRequest *rqst = new SegmentRequest(rqstname);
    rqst->setVideoId(data->at(3).c_str());
    rqst->setSegmentId(i);
    rqst->setClientId(clientId);
    rqst->setUploader(data->at(9).c_str());
    rqst->setCategory(data->at(10).c_str());
    rqst->setSize(std::stoi(data->at(11)));
    return rqst;
}

/*
 * @brief sends a request on a specific channel
 *
 * @param rqst the request to be sent
 */
void Client::sendRequest(SegmentRequest *rqst) {
    send(rqst, "reverseProxyOut");
}

/*
 * @brief gets the requests for the client
 *
 * In the nwInfo the client can get its requests with the userId
 */
void Client::getRequests() {
    requestSet = nwInfo->getRequestsForClient(userId);
}

/*
 * @brief is called at the end of the program execution
 *
 * used for garbage collection
 */
void Client::finish() {
    delete nextRequestEvent;
}
