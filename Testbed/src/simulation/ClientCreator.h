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

#ifndef SRC_SIMULATION_CLIENTCREATOR_H_
#define SRC_SIMULATION_CLIENTCREATOR_H_
#include <vector>
#include <string>
#include <omnetpp.h>

class ClientCreator {
public:
    void createClients();
    void createClientsWithHubs();
    ClientCreator(std::vector<std::string>* userIds, omnetpp::cModule* parent,
            omnetpp::cModule* reverseProxy);
    virtual ~ClientCreator();
protected:
    std::vector<std::string>* userIds;
    std::vector<omnetpp::cModule*> hubs;
    omnetpp::cModule* parent;
    omnetpp::cModule* reverseProxy;
};

#endif /* SRC_SIMULATION_CLIENTCREATOR_H_ */
