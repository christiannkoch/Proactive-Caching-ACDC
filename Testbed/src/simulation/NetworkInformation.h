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

#ifndef SRC_SIMULATION_NETWORKINFORMATION_H_
#define SRC_SIMULATION_NETWORKINFORMATION_H_
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <omnetpp.h>
#include <DBConnection.h>
#include <sstream>
#include <fstream>
#include <map>
#include "DatabaseInformation.h"
#include "ProxyCachseSettingst.h"

using namespace omnetpp;

class NetworkInformation {
public:
    static NetworkInformation* getInformation(omnetpp::cModule* owner);
    //static void clearNetworkInformation();
    int getAmountOfReverseProxys();
    int getAmountOfClients();
    void setupNetworkInformation();
    std::vector<std::string>* getClientIdsForReverseProxy(
            std::string proxyName);

    double getFirstRequestTimed();
    std::vector<ProxyCacheSettings_t>* getProxyCacheSettings();
    ProxyCacheSettings_t* getSettings(int id);
    std::string getAdmissionStrategy(int id);
    std::string getEvictionStrategy(int id);
    DBConnection* getDatabaseConnection();
    std::vector<std::vector<std::string>*>* getRequestsForClient(int id);
    void increaseHit();
    int getHits();
    int getAmountOfLeafProxys();
    int getAmountOfLevels();
    bool isConnected(int id, std::string proxy);
    std::map<int, std::vector<int>*>* getConnectionTable();
    std::map<std::string, std::vector<std::string>*>* getLeafProxyVector();
    int getGateSize(int id);
    virtual ~NetworkInformation();
    double getEndTime();
    double getSimulationDuration();
    int getCDNDelay();
protected:
    int cdnDelay = 0;
    int amountOfLevels = 1;
    NetworkInformation(omnetpp::cModule* owner);

    static NetworkInformation* singleton;
    omnetpp::cModule* owner;

    std::vector<std::string>* userIds;
    std::vector<std::string>* dbParameters;
    std::vector<std::string>* reverseProxyParameters;
    double firstRequestTime;

    std::vector<int> levels;
    std::map<int, std::vector<std::vector<std::string>*>*> allRequests;
    int hits = 0;

    std::vector<std::string> config;

    // Databse Parameters and functions
    std::string path;
    struct DatabseInformation_t databaseInformation;
    DBConnection* dataBaseConnection;
    void setupDatabaseParameters(std::vector<std::string>* config);
    DBConnection* connectToDatabase();

    // Helper Functions
    void readFile();
    std::string removeSpaces(std::string s);
    std::vector<std::string> splitString(std::string s,
            const std::string& delimiter);
    bool toBool(std::string s);

    //proxy functions and variables
    std::vector<ProxyCacheSettings_t> proxyCacheSettings;
    void generateUserIdsPerProxy();
    void generateProxyCacheSettings();
    std::vector<std::vector<std::string>*>* userIdsPerProxy = new std::vector<
            std::vector<std::string>*>();

    std::vector<std::pair<double, double>>* generateVectorOfDoublePairs(
            std::vector<std::string> values);

    // Networkcreation
    int amountOfReverseProxys;
    int amountOfClients;
    void selectAllUsers();
    void setupAllRequests();
    void getStartTime();
    void generateConnectionTable();
    std::map<int, std::vector<int>*> connectionTable;
    std::map<std::string, std::vector<std::string>*> leafProxyVector;

};

#endif /* SRC_SIMULATION_NETWORKINFORMATION_H_ */
