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

#include "Client.h"
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "DatabaseFactory.h"

Define_Module(Client);

using namespace omnetpp;

void Client::initialize() {
    this->nwInfo = NetworkInformation::getInformation(this);
    this->userId = par("userId");
    this->clientId = par("clientId");
    getRequests();
    if (!requestSet->empty()) {
        result = requestSet->at(requestSet->size() - 1);
        requestSet->pop_back();
        nextRequestEvent = new cMessage("next Request");
        periodicRequest = new cMessage("periodic Request");
        videoDuration = std::atof((result->at(4).c_str())); //Das ist normalerweise das richtige
        //videoDuration = std::atof((result->at(7).c_str())); //für den direkten vergleich mit markus' set
        chunkDuration = 5.0;        //std::atof((result->at(5).c_str()));

        if (videoDuration > std::atof((result->at(7).c_str()))) //Das ist normalerweise das richtige
            videoDuration = std::atof((result->at(7).c_str()));
        /*if (chunkDuration < 1)
         chunkDuration = 5.4;*/
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

void Client::handleMessage(cMessage *msg) {

    if (msg == nextRequestEvent) {
        counter = 1;
        sendRequest(generateRequest(result, counter));
        counter++;
        //std::cout << "erstes mal Request event" << "\n";
        scheduleAt(simTime().dbl() + (chunkDuration - 1), periodicRequest);
    } else {
        if (msg == periodicRequest) {
            //std::cout << "periodischer Request" << "\n";
            if (numberOfRequests > counter) {
                //std::cout
                // << "periodische Request gesendet und noch welche übrig"
                //<< "\n";
                sendRequest(generateRequest(result, counter));
                counter++;
                scheduleAt(simTime().dbl() + (chunkDuration - 1), msg);
            } else {
                //std::cout << "nächstes Requestset" << "\n";
                delete result;
                //std::cout << "altes Result gelöscht" << "\n";
                if (!requestSet->empty()) {
                    //std::cout << "es gab das nächste Requestset" << "\n";
                    result = requestSet->at(requestSet->size() - 1);
                    requestSet->pop_back();
                    videoDuration = std::atof((result->at(4).c_str()));
                    if (videoDuration > std::atof((result->at(7).c_str())))
                        videoDuration = std::atof((result->at(7).c_str()));
                    numberOfRequests = videoDuration / chunkDuration;
                    if (numberOfRequests < 1)
                        numberOfRequests = 1;
                    //std::cout << "nächstes Reqeustevent wird gescheduled"
                    //<< "\n";
                    //std::cout << "jetzt ist es" << simTime().dbl() << "\n";
                    //std::cout << "nächste Zeit ist"
                    //<< std::atof(result->at(1).c_str())
                    //- nwInfo->getFirstRequestTimed() << "\n";
                    //std::cout << nextRequestEvent->getName() << "\n";

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

                    //std::cout << "ist gescheduled" << "\n";
                } else {
                    //std::cout << "Client ist fertig" << "\n";
                    delete requestSet;
                    delete msg;
                }
            }

        } else {
            //std::cout << "Hier kommen wir eigentlich gar nicht rein" << "\n";
            delete msg;
        }
    }

}

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

void Client::sendRequest(SegmentRequest *rqst) {
//bubble("Send Request");
    send(rqst, "reverseProxyOut");
}

void Client::getRequests() {
    requestSet = nwInfo->getRequestsForClient(userId);
}

void Client::finish() {
    delete nextRequestEvent;
}
