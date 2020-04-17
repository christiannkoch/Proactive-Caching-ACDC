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
/* @file Client.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the Client
 *
 * @section DESCRIPTION
 */
#ifndef SRC_SIMULATION_CLIENT_H_
#define SRC_SIMULATION_CLIENT_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "../database/DatabaseFactory.h"
#include "../database/DBConnection.h"
#include "NetworkInformation.h"

using namespace omnetpp;

class Client: public cSimpleModule {
protected:
    SegmentRequest *generateRequest(std::vector<std::string>* data, int i);
    void sendRequest(SegmentRequest *rqst);
    void initialize() override;
    void handleMessage(cMessage *msg) override;
    void finish() override;
    void getRequests();
    int clientId;
    int userId;
    std::vector<std::vector<std::string>*>* requestSet;
    std::vector<std::string>* result;
    cMessage* nextRequestEvent;
    cMessage* periodicRequest;
    NetworkInformation* nwInfo;
    double videoDuration;
    double chunkDuration;
    int numberOfRequests;
    int counter;
};

#endif /* SRC_SIMULATION_CLIENT_H_ */
