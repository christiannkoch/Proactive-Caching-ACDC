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

#ifndef SRC_SIMULATION_HUB_H_
#define SRC_SIMULATION_HUB_H_

#include <omnetpp.h>
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "RequestHandler.h"

using namespace omnetpp;

class Hub : public cSimpleModule {
protected:
    void initialize() override;
    void handleMessage(cMessage *msg) override;
    void finish() override;
    void sendMessage(cMessage *msg, int gate);
    RequestHandler* rqHandler;
};

#endif /* SRC_SIMULATION_HUB_H_ */
