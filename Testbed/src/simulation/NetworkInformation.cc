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
/* @file NetworkInformation.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the NetworkInformation
 *
 * @section DESCRIPTION
 *
 * The NetworkInformation is a central class that holds much data for the simulation.
 */
#include "NetworkInformation.h"
#include <DatabaseFactory.h>
#include <vector>
#include <string>
#include <omnetpp.h>
#include <utility>
#include <ctime>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <boost/lexical_cast.hpp>

NetworkInformation* NetworkInformation::singleton = 0; // since we only want one instance of this class to exist, we do it with this singleton

using namespace omnetpp;
/*
 * @brief returns an instance of the NetworkInformation class
 *
 * @param owner the module that created the instance of the NetworkInformation.
 *
 * The NetworkInformation class is responsible for holding, creating and serving important information
 * for the simulation like the database with the request set.
 */
NetworkInformation::NetworkInformation(omnetpp::cModule* owner) {
    this->owner = owner;
    std::string test = owner->getParentModule()->par("SetupfileName");
    std::string temp = owner->getParentModule()->par("Runnumber");
    runNumber = temp;
    path = "configurations/" + test;
    readFile();
    setupDatabaseParameters(&config);
    this->dataBaseConnection = connectToDatabase();

}

/*
 * @brief opens the connection to the database
 *
 * @return the connection to the database
 */
DBConnection* NetworkInformation::connectToDatabase() {
    std::string addressBuilder = "tcp://" + databaseInformation.address + ":";
    return DatabaseFactory::getConnection(databaseInformation.type,
            databaseInformation.login, databaseInformation.password,
            addressBuilder, databaseInformation.name, databaseInformation.port);
}

/*
 * @brief returns the end time of the simulation
 *
 * @return the simulation end time as a unix timestamp
 */
double NetworkInformation::getEndTime() {
    return this->databaseInformation.endTime;
}

/*
 * @brief returns the duration of the simulation
 *
 * @return the duration of the simulation in seconds
 */
double NetworkInformation::getSimulationDuration() {
    return (this->databaseInformation.endTime
            - this->databaseInformation.startTime);
}

/*
 * @brief sets up the database parameter
 *
 * the database parameters are used to connect to the database
 */
void NetworkInformation::setupDatabaseParameters(
        std::vector<std::string>* config) {
    databaseInformation.type = removeSpaces(
            splitString(config->at(0), "=").at(1));
    databaseInformation.name = removeSpaces(
            splitString(config->at(1), "=").at(1));
    databaseInformation.table = removeSpaces(
            splitString(config->at(2), "=").at(1));
    databaseInformation.login = removeSpaces(
            splitString(config->at(3), "=").at(1));
    databaseInformation.password = removeSpaces(
            splitString(config->at(4), "=").at(1));
    databaseInformation.address = removeSpaces(
            splitString(config->at(5), "=").at(1));
    databaseInformation.port = std::stoi(
            removeSpaces(splitString(config->at(6), "=").at(1)));
    databaseInformation.startTime = std::atof(
            removeSpaces(splitString(config->at(7), "=").at(1)).c_str());
    databaseInformation.endTime = std::atof(
            removeSpaces(splitString(config->at(8), "=").at(1)).c_str());
    databaseInformation.category = removeSpaces(
            splitString(config->at(9), "=").at(1));
    config->erase(config->begin(), config->begin() + 10);

}

/*
 * @brief removes spaces from a given string
 *
 * @param s the string from which to remove the spaces
 *
 * @return a string without spaces
 */
std::string NetworkInformation::removeSpaces(std::string s) {
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    return s;
}

/*
 * @brief splits a string into a vector of strings based on a delimiter
 *
 * @param s the string to split
 * @param delimiter the delimiter that signals a split
 *
 * the brief description says it all
 *
 * @return a vector of strings
 */
std::vector<std::string> NetworkInformation::splitString(std::string s,
        const std::string& delimiter) {
    std::vector < std::string > tokens;
    size_t pos = 0;
    std::string token;

    int i = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        i++;
        s.erase(0, pos + delimiter.length());
    }

    tokens.push_back(s);
    return tokens;
}

NetworkInformation::~NetworkInformation() {
    for (auto connection : connectionTable)
        delete connection.second;
    delete dataBaseConnection;

}

/*
 * @brief returns the singleton of the NetworkInformation
 *
 * @return the Networkinformation
 */

NetworkInformation* NetworkInformation::getInformation(
        omnetpp::cModule* owner) {
    if ((singleton == 0))
        singleton = new NetworkInformation(owner);
    return singleton;
}

/*
 * @brief is called when the NetworkInformation is initialized
 *
 */
void NetworkInformation::setupNetworkInformation() {
    generateProxyCacheSettings();
    this->amountOfReverseProxys = proxyCacheSettings.size();
    generateConnectionTable();
    selectAllUsers();

    amountOfClients = userIds->size();
    generateUserIdsPerProxy();
    getStartTime();
    setupAllRequests();
}

/*
 * @brief performs a database request to get all requests
 *
 * Here the database request is performed to get all requests. the requests are stored
 * for further use
 */
void NetworkInformation::setupAllRequests() {

    std::cout << "setting up all Requests, may take some time" << endl;
    const clock_t begin_time = clock();

    DBCursor* dbCursor = dataBaseConnection->selectAllRequests(
            databaseInformation);
    std::vector<std::vector<std::string>*>* resultAllRequests =
            dbCursor->getResultSet();
    std::cout << "Reading the Database done in "
            << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;

    for (auto user = userIds->begin(); user != userIds->end(); ++user) {
        std::string atm = (*user);
        std::vector<std::vector<std::string>*>* vectorContainer =
                new std::vector<std::vector<std::string>*>();
        allRequests[std::stoi(*user)] = vectorContainer;
    }

    std::cout << "generated the empty map in "
            << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
    int i = 0;
    for (auto item = resultAllRequests->begin();
            item != resultAllRequests->end(); ++item) {

        allRequests[std::stoi((*item)->at(2))]->push_back(*item);

        i++;
    }

    delete userIds;
    delete dbCursor;
    delete resultAllRequests;
    std::cout << "Completed Setup in "
            << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
}

/*
 * @brief returns the requests for a client
 * @param id the id of the client we want the requests for
 * @return a vector of a vector of strings which holds all the information of a request for a client
 */
std::vector<std::vector<std::string>*>* NetworkInformation::getRequestsForClient(
        int id) {
    return this->allRequests[id];

}

/*
 * @brief returns the connection to the database
 * @return the connection to the database
 */
DBConnection* NetworkInformation::getDatabaseConnection() {
    return this->dataBaseConnection;
}

/*
 * @brief performs a database request to get the start time
 */
void NetworkInformation::getStartTime() {
    DBCursor* dbCursor = dataBaseConnection->selectFirstTimestamp(databaseInformation);
    std::vector<std::vector<std::string>*>* resultFristTimestamp =
            dbCursor->getResultSet();
    firstRequestTime = std::atof(resultFristTimestamp->at(0)->at(0).c_str());
    delete dbCursor;
    delete resultFristTimestamp->at(0);
    delete resultFristTimestamp;
}

/*
 * @brief performs a database request to get all distinct client ids
 */
void NetworkInformation::selectAllUsers() {


    DBCursor* dbCursor = dataBaseConnection->selectAllClients(databaseInformation);
    std::vector<std::vector<std::string>*>* resultSet =
            dbCursor->getResultSet();
    userIds = resultSet->at(0);
    delete dbCursor;
    delete resultSet;
}

/*
 * @brief returns the client ids connected to a reverse proxy
 * @param proxyName the name of the proxy we want the clientIds for
 * @return a vector of strings which represents a list of client ids that will be connected to a proxy
 */
std::vector<std::string>* NetworkInformation::getClientIdsForReverseProxy(
        std::string proxyName) {
    return leafProxyVector[proxyName];
}

/*
 * @brief returns the cache settings for a proxy
 * @reutrn the cache settings for a proxy
 */
std::vector<ProxyCacheSettings_t>* NetworkInformation::getProxyCacheSettings() {
    return &proxyCacheSettings;
}

/*
 * @brief signals if two proxies are connected
 *
 * If a connection in the connection table exists true is returned.
 *
 * @return a boolean that is true if two proxies are connected, false otherwise
 */
bool NetworkInformation::isConnected(int id, std::string proxy) {
    for (unsigned int i = 0; i < connectionTable[id]->size(); i++)
        if (proxyCacheSettings[connectionTable[id]->at(i)].name == proxy)
            return true;
    return false;
}

/*
 * @brief returns the admission strategy of a proxy
 *
 * @param id the id of the proxy we want the admission strategy from
 * @return a string that represents the admission strategy
 */
std::string NetworkInformation::getAdmissionStrategy(int id) {
    return proxyCacheSettings[id - 1].admissionStrategy;
}

/*
 * @brief returns the eviction strategy of a proxy
 *
 * @param id the id of the proxy we want the eviction strategy from
 * @return a string that represents the eviction strategy
 */
std::string NetworkInformation::getEvictionStrategy(int id) {
    return proxyCacheSettings[id - 1].evictionStrategy;
}

/*
 * @brief a function that assignes user ids to the leaf proxies by random
 *
 * the brief says it all
 */
void NetworkInformation::generateUserIdsPerProxy() {
    unsigned int share = amountOfClients / getAmountOfLeafProxys();
    unsigned int ueberhang = amountOfClients % getAmountOfLeafProxys();
    //int j = 0;

    for (int i = 0; i <= getAmountOfLeafProxys(); ++i) {
        std::vector < std::string > *newVec = new std::vector<std::string>();
        userIdsPerProxy->push_back(newVec);
    }
    srand (time(NULL));for
(    auto id : *userIds) {
        bool canNotConnect = true;
        int toConnect = 0;
        while (canNotConnect) {
            unsigned int tryThis = rand() % getAmountOfLeafProxys();
            if (tryThis <= ueberhang - 1) {
                if (userIdsPerProxy->at(tryThis)->size() < (share + 1)) {
                    toConnect = tryThis;
                    canNotConnect = false;
                }
            } else {
                if (userIdsPerProxy->at(tryThis)->size() < share) {
                    toConnect = tryThis;
                    canNotConnect = false;
                }
            }

        }
        userIdsPerProxy->at(toConnect)->push_back(id);
    }
    int l = 0;
    for (unsigned int i = 0; i < proxyCacheSettings.size(); i++) {
        if (proxyCacheSettings[i].leaf) {
            leafProxyVector[proxyCacheSettings[i].name] = userIdsPerProxy->at(
                    l);
            l++;
        }

    }
}

/*
 * @brief generates the cache settings for each proxy
 *
 * the brief says it all
 */
void NetworkInformation::generateProxyCacheSettings() {
    for (auto line : config) {
        if (line[0] == 'R') {
            //std::string valueString = splitString(line, ":").at(1);
            std::vector < std::string > values = splitString(line, ";");
            proxyCacheSettings.emplace_back(
                    removeSpaces(splitString(values.at(0), "=").at(1)),
                    removeSpaces(splitString(values.at(2), "=").at(1)),
                    std::stoll(
                            removeSpaces(splitString(values.at(3), "=").at(1))),
                    splitString(
                            removeSpaces(splitString(values.at(4), "=").at(1)),
                            ","),
                    splitString(
                            removeSpaces(splitString(values.at(1), "=").at(1)),
                            ","),
                    removeSpaces(splitString(values.at(5), "=").at(1)),
                    generateVectorOfDoublePairs(
                            splitString(
                                    removeSpaces(
                                            splitString(values.at(6), "=").at(
                                                    1)), ",")),
                    removeSpaces(splitString(values.at(7), "=").at(1)),
                    toBool(removeSpaces(splitString(values.at(8), "=").at(1))));

        }
    }
    config.erase(config.begin(), config.begin() + proxyCacheSettings.size());
}

/*
 * @brief takes the string of storage alterations and generates a vector of double pairs from it
 *
 * this is basically parsing the the strings to double pairs
 *
 * @return a vector of double pairs
 */
std::vector<std::pair<double, double>>* NetworkInformation::generateVectorOfDoublePairs(
        std::vector<std::string> values) {
    std::vector<std::pair<double, double>>* storageAlterationVector =
            new std::vector<std::pair<double, double>>();
    if (!(values.at(values.size() - 1) == "")) {
        for (auto i : values) {
            std::vector < std::string > value = splitString(removeSpaces(i),
                    ":");
            storageAlterationVector->emplace_back(
                    std::make_pair(std::stod(value.at(0)),
                            std::stod(value.at(1))));
        }
    }

    return storageAlterationVector;
}

/*
 * @brief generates the connection Table
 *
 * the brief description says it all
 */
void NetworkInformation::generateConnectionTable() {
    std::string line = config[0];
    this->cdnDelay = std::stoi(removeSpaces(splitString(line, "=").at(1)));
    config.erase(config.begin());
    line = config[0];
    this->amountOfLevels = std::stoi(
            removeSpaces(splitString(line, "=").at(1)));
    config.erase(config.begin());
    config.erase(config.begin());
    for (unsigned int i = 0; i < config.size(); i++) {
        std::vector < std::string > column = splitString(config[i], " ");
        column.erase(column.begin());
        std::vector<int>* connections = new std::vector<int>();
        for (unsigned int j = 0; j < column.size(); j++) {
            if (column[j] == "+")
                connections->push_back(j);
        }
        connectionTable[i] = connections;
    }
}

/*
 * @brief parses a string to a boolean value if the string is "true" true is returned. False otherwise
 *
 * brief says it all
 *
 * @return a boolean value
 */
bool NetworkInformation::toBool(std::string s) {
    if (s == "true")
        return true;
    return false;
}

/*
 * @brief retrurns the amount of Leaf Proxies
 *
 * the amount of leaf proxies is used to calculate how many clients are connected to a single reverse proxy
 *
 * @return the amount of leaf proxies
 */
int NetworkInformation::getAmountOfLeafProxys() {
    int amountOfLeafProxys = 0;
    for (auto i : proxyCacheSettings)
        if (i.leaf)
            amountOfLeafProxys++;
    return amountOfLeafProxys;
}

/*
 * @brief returns the connection table
 *
 * brief says it all
 *
 * @return the connection table
 */
std::map<int, std::vector<int>*>* NetworkInformation::getConnectionTable() {
    return &connectionTable;
}

/*
 * @brief returns map that assigns each leaf proxy vector its client ids
 *
 * in the leafProxyVector a proxy id is desigend a vector of client strings.
 * Maybe the name is misleading
 *
 * @return a list of client ids assigned to a proxy id
 */
std::map<std::string, std::vector<std::string>*>* NetworkInformation::getLeafProxyVector() {
    return &leafProxyVector;
}

/*
 * @briefs returns the amount of reverse proxies
 *
 * brief says it all
 *
 * @return the amount of reverse proxies
 */
int NetworkInformation::getAmountOfReverseProxys() {
    return this->amountOfReverseProxys;
}

/*
 * @brief returns the amount of clients
 *
 * brief says it all
 *
 * @return the amount of clients
 */
int NetworkInformation::getAmountOfClients() {
    return this->amountOfClients;
}

/*
 * @brief returns the gate size of a proxy
 *
 * @param id the id of the proxy we want to know the gate size of
 *
 * @return the gate size of a proxy
 */
int NetworkInformation::getGateSize(int id) {
    return connectionTable[id]->size();
}

/*
 * @brief returns the time of the first request
 *
 * the time is used in order to know when to start the simulation
 *
 * @return a double value as a unix timestamp that represents the time of the first request
 */
double NetworkInformation::getFirstRequestTimed() {
    return this->firstRequestTime;
}

/*
 * @brief increments the amount of hits by 1 (is never used)
 */
void NetworkInformation::increaseHit() {
    this->hits = hits + 1;
}

/*
 * @brief returns the amount of hits (is never used)
 *
 * @return the amount of hits
 */
int NetworkInformation::getHits() {
    return this->hits;
}

/*
 * @brief returns the amount of levels
 *
 * the amount of levels of the topology is returned
 *
 * @return the amount of levels
 */
int NetworkInformation::getAmountOfLevels() {
    return this->amountOfLevels;
}

/*
 * @brief returns the inter-cdn delay
 *
 * used with the amount of levels to calculate the delay of the channels between the reverse proxies
 *
 * @return the inter-cdn delay
 */
int NetworkInformation::getCDNDelay() {
    return this->cdnDelay;
}

/*
 * @brief a function that reads a text file line by line
 *
 * brief says it all
 */
void NetworkInformation::readFile() {
    std::ifstream infile(path);
    std::string line;
    while (getline(infile, line)) {
        config.push_back(line);
    }
}

/*
 * @brief returns the cache settings for a reverse proxy
 * @param id the id of the reverse proxy we want the cache settings for
 *
 * @return the cache settings for a reverse proxy
 */
ProxyCacheSettings_t* NetworkInformation::getSettings(int id) {
    return &proxyCacheSettings.at(id - 1);
}

std::string NetworkInformation::getRunNumber(){
    return this->runNumber;
}

