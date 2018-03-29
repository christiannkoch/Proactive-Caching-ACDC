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

#include "ClientCreator.h"
#include <omnetpp.h>
#include <string>
#include "Hub.h"

using namespace omnetpp;
/*
 * @brief creates an instance of the ClientCreator
 *
 * @param userIds the userIds of the clients that have to be created
 * @param the network on which the clients are created
 * @param reverseProxy the owner of the instance of the ClientCreator
 */
ClientCreator::ClientCreator(std::vector<std::string>* userIds,
        omnetpp::cModule* parent, omnetpp::cModule* reverseProxy) {
    this->userIds = userIds;
    this->parent = parent;
    this->reverseProxy = reverseProxy;
}

ClientCreator::~ClientCreator() {
}

/*
 * @brief creates clients with Hubs in between
 *
 * Omnet only allows a module to connect to a specific number of other modules.
 * with this function we basically split the set of clients in half and assign
 * each subset of clients a connector that then connects these clients to the reverseProxy.
 *
 * NOTE: this is only implemented, so that the maximum number times two of connections can be handled.
 * This has to be changed in the future to allow a infinite number of connectinos.
 */
void ClientCreator::createClientsWithHubs() {
    for (unsigned int i = 0; i < 2; i++) {
        cModuleType *moduleType = cModuleType::get(
                "testbed.src.simulation.Hub");
        cModule *module = moduleType->create("Hub", parent);
        reverseProxy->gate("clientOut", i)->connectTo(module->gate("proxyIn"));
        module->gate("proxyOut")->connectTo(reverseProxy->gate("clientIn", i));
        if ((this->userIds->size() % 2 == 1) && i == 0) {
            module->setGateSize("clientIn", (userIds->size() / 2) + 1);
            module->setGateSize("clientOut", (userIds->size() / 2) + 1);
        } else {
            module->setGateSize("clientIn", (userIds->size() / 2));
            module->setGateSize("clientOut", (userIds->size() / 2));
        }
        module->finalizeParameters();
        module->buildInside();
        module->scheduleStart(simTime());
        hubs.push_back(module);
    }
    int j = 0;
    int k = 0;
    for (unsigned int i = 0; i < this->userIds->size(); i++) {

        cModuleType *moduleType = cModuleType::get(
                "testbed.src.simulation.Client");
        cModule *module = moduleType->create("Client", parent);

        /*
         * We can create the channels for the clients which are connected to the
         * reverseProxy with delays. We did not do this but it is possible.
         */


        /* cDelayChannel *channelTop = cDelayChannel::create("channelTop");
        channelTop->setDelay(0.1);

        cDelayChannel *channelBot = cDelayChannel::create("channelBot");
        channelBot->setDelay(0.1);*/
        if (i % 2 == 0) {
            hubs[0]->gate("clientOut", j)->connectTo(
                    module->gate("reverseProxyIn")/*, channelTop*/);
            module->gate("reverseProxyOut")->connectTo(
                    hubs[0]->gate("clientIn", j)/*, channelBot*/);
            j++;
        } else {
            hubs[1]->gate("clientOut", k)->connectTo(
                    module->gate("reverseProxyIn")/*, channelTop*/);
            module->gate("reverseProxyOut")->connectTo(
                    hubs[1]->gate("clientIn", k)/*, channelBot*/);
            k++;
        }
        /*channelTop->callInitialize();
        channelBot->callInitialize();*/

        module->par("userId") = std::stoi(this->userIds->at(i));
        module->par("clientId") = i;
        module->finalizeParameters();
        module->buildInside();
        module->scheduleStart(simTime());

    }
}

/*
 * @brief creates clients that connect to the reverseProxy
 *
 */
void ClientCreator::createClients() {
    for (unsigned int i = 0; i < this->userIds->size(); i++) {

        cModuleType *moduleType = cModuleType::get(
                "testbed.src.simulation.Client");
        cModule *module = moduleType->create("Client", parent);

        /*cDelayChannel *channelTop = cDelayChannel::create("channelTop");
        channelTop->setDelay(0.1);

        cDelayChannel *channelBot = cDelayChannel::create("channelBot");
        channelBot->setDelay(0.1);*/

        reverseProxy->gate("clientOut", i)->connectTo(
                module->gate("reverseProxyIn")/*, channelTop*/);
        module->gate("reverseProxyOut")->connectTo(
                reverseProxy->gate("clientIn", i)/*, channelBot*/);

        /*channelTop->callInitialize();
        channelBot->callInitialize();*/

        module->par("userId") = std::stoi(this->userIds->at(i));
        module->par("clientId") = i;
        module->finalizeParameters();
        module->buildInside();
        module->scheduleStart(simTime());

    }
}
