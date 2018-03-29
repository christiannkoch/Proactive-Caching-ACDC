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
/* @file ReverseProxyCreator.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the ReverseProxyCreator
 *
 * @section DESCRIPTION
 */
#ifndef SRC_SIMULATION_REVERSEPROXYCREATOR_H_
#define SRC_SIMULATION_REVERSEPROXYCREATOR_H_
#include <vector>
#include <string>
#include <omnetpp.h>

#include "../cache/ProxyCacheSettingst.h"
#include "NetworkInformation.h"

class ReverseProxyCreator {
public:
    ReverseProxyCreator(cModule* parent, cModule* originServer, int userIDs,
            int amountOfLeafProxies, int amountOfLevels, int cdnDelay,
            std::vector<ProxyCacheSettings_t>* cacheSettings,
            std::map<std::string, std::vector<std::string>*>* leafProxyVector,
            std::map<int, std::vector<int>*>* connectionTable);
    void createReverseProxys();
    virtual ~ReverseProxyCreator();
protected:
    int amountOfLevels;
    int cdnDelay;
    double channelDelay;
    omnetpp::cModule* parent;
    omnetpp::cModule* originServer;
    NetworkInformation* nwInfo;
    int getAmountOfClientGates(std::string proxyName);
    void connectNetwork();
    int userIds;
    std::vector<cModule*> ReverseProxys;
    std::vector<ProxyCacheSettings_t>* cacheSettings;
    int clientsCreated = 0;
    long long firstRequest = 0;
    int amountOfLeafProxys;
    int getGateSize(int id);
    std::vector<int>* getConnectionsForProxy(int id);
    int getGate(int id, std::string proxy);
    std::map<std::string, std::vector<std::string>*>* leafProxyVector;
    std::map<int, std::vector<int>*>* connectionTable;
};

#endif /* SRC_SIMULATION_REVERSEPROXYCREATOR_H_ */
