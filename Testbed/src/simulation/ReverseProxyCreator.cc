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
/* @file ReverseProxyCreator.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the ReverseProxyCreator
 *
 * @section DESCRIPTION
 *
 * creates reverse proxies in the network
 */
#include "ReverseProxyCreator.h"
#include <omnetpp.h>

#include "../cache/ProxyCacheSettingst.h"

using namespace omnetpp;

/*
 * @brief returns an instance of the ReverseProxyCreator
 * @param parent the module on which the reverse proxies are created on
 * @param originServer the module that created the instance of the ReverseProxyCreator
 * @param userIDs all userIDs
 * @param amountOfLeafProxies the amount of leaf proxies
 * @param amountOfLevels the amount of levels
 * @param cdnDelay the inter-cdn delay
 * @param leafProxyVector a map that assigns each leaf proxy its client ids
 * @param connectionTable the connection table
 * @return an instrance of the ReverseProxyCreator
 */
ReverseProxyCreator::ReverseProxyCreator(cModule* parent, cModule* originServer,
        int userIDs, int amountOfLeafProxies, int amountOfLevels, int cdnDelay,
        std::vector<ProxyCacheSettings_t>* cacheSettings,
        std::map<std::string, std::vector<std::string>*>* leafProxyVector,
        std::map<int, std::vector<int>*>* connectionTable) {
    // TODO Auto-generated constructor stub

    this->userIds = userIDs;
    this->amountOfLeafProxys = amountOfLeafProxies;
    this->parent = parent;
    this->originServer = originServer;
    this->cacheSettings = cacheSettings;
    this->leafProxyVector = leafProxyVector;
    this->connectionTable = connectionTable;
    this->channelDelay = ((double) cdnDelay / (double) amountOfLevels) / 1000/2;
}

ReverseProxyCreator::~ReverseProxyCreator() {
}

/*
 * @brief creates the reverse proxies on the network
 *
 * Creates the reverse proxies
 */
void ReverseProxyCreator::createReverseProxys() {
    int proxyId = 1;
    for (auto revProxy : *cacheSettings) {
        cModuleType *moduleType = cModuleType::get(
                "testbed.src.simulation.ReverseProxy");
        cModule *module = moduleType->create(revProxy.name.c_str(), parent);
        module->setGateSize("proxyIn", getGateSize(proxyId));
        module->setGateSize("proxyOut", getGateSize(proxyId));
        if (revProxy.leaf) {
            if (getAmountOfClientGates(revProxy.name) > 400000) {
                module->setGateSize("clientIn", 2);
                module->setGateSize("clientOut", 2);
            } else {
                module->setGateSize("clientIn",
                        getAmountOfClientGates(revProxy.name));
                module->setGateSize("clientOut",
                        getAmountOfClientGates(revProxy.name));
            }
        }
        module->par("proxyId") = proxyId;
        module->par("name") = revProxy.name;
        module->finalizeParameters();
        module->buildInside();
        module->scheduleStart(simTime());

        ReverseProxys.push_back(module);
        proxyId++;
    }
    connectNetwork();
}

/*
 * @brief connects the reverse proxies according to the connection table
 */
void ReverseProxyCreator::connectNetwork() {
    for (auto proxy : ReverseProxys) {
        std::vector<int>* connections = getConnectionsForProxy(
                proxy->getId() - 2);
        for (unsigned int i = 0; i < connections->size(); i++) {
            std::string test2 = cacheSettings->at(proxy->getId() - 3).name;
            if (connections->at(i) == 0) { // origin server connects to proxy
                cDelayChannel *channelTop = cDelayChannel::create("channelTop");
                channelTop->setDelay(channelDelay);

                cDelayChannel *channelBot = cDelayChannel::create("channelBot");
                channelBot->setDelay(channelDelay);
                proxy->gate("proxyOut", i)->connectTo(
                        originServer->gate("originGateIn",
                                getGate(0,
                                        cacheSettings->at(proxy->getId() - 3).name)),
                        channelTop);
                originServer->gate("originGateOut",
                        getGate(0, cacheSettings->at(proxy->getId() - 3).name))->connectTo(
                        proxy->gate("proxyIn", i), channelBot);
                channelTop->callInitialize();
                channelBot->callInitialize();
            } else { // proxy connects to proxy
                cDelayChannel *channelBot = cDelayChannel::create("channelBot");
                channelBot->setDelay(channelDelay);
                proxy->gate("proxyOut", i)->connectTo(
                        ReverseProxys[connections->at(i) - 1]->gate("proxyIn",
                                getGate(
                                        ReverseProxys[connections->at(i) - 1]->getId()
                                                - 2,
                                        cacheSettings->at(proxy->getId() - 3).name)),
                        channelBot);
                channelBot->callInitialize();
            }

        }
    }
}

/*
 * @brief returns the amount of client gates for a given proxy
 * @param proxyName the name of the proxy we need the amount of gates for
 * @return the amount of client gates
 */
int ReverseProxyCreator::getAmountOfClientGates(std::string proxyName) {
    return leafProxyVector->at(proxyName)->size();
}

/*
 * @brief returns the gate size for a given proxy
 * @param id the id of the proxy we need the gate size for
 * @return the gate size of a proxy
 */
int ReverseProxyCreator::getGateSize(int id) {
    return connectionTable->at(id)->size();
}

/*
 * @brief returns the connections for a proxy
 * @param id the id of a proxy we want the connections for
 * @return the list of proxy ids the given proxy is connected to
 */
std::vector<int>* ReverseProxyCreator::getConnectionsForProxy(int id) {
    return connectionTable->at(id);
}

/*
 * @brief returns the gate a proxy connects to another proxy
 * @param id the id of a proxy we want the connection for
 * @param proxy the proxy we want to connect to the proxy identified by id
 * @return the gate where to connect the proxy
 */
int ReverseProxyCreator::getGate(int id, std::string proxy) {

    for (unsigned int i = 0; i < connectionTable->at(id)->size(); i++) {
        if (!(connectionTable->at(id)->at(i) == 0)
                && (cacheSettings->at(connectionTable->at(id)->at(i) - 1).name
                        == proxy))
            return i;

    }
    throw "Die Eingabematrix muss falsch sein";
}
