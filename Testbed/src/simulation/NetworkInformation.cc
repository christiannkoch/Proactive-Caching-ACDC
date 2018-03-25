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

NetworkInformation* NetworkInformation::singleton = 0;

using namespace omnetpp;

NetworkInformation::NetworkInformation(omnetpp::cModule* owner) {
    this->owner = owner;
    std::string test = owner->getParentModule()->par("SetupfileName");
    path = "configurations/" + test;
    readFile();
    setupDatabaseParameters(&config);
    this->dataBaseConnection = connectToDatabase();

}

DBConnection* NetworkInformation::connectToDatabase() {
    std::string addressBuilder = "tcp://" + databaseInformation.address + ":";
    return DatabaseFactory::getConnection(databaseInformation.type,
            databaseInformation.login, databaseInformation.password,
            addressBuilder, databaseInformation.port);
}

double NetworkInformation::getEndTime() {
    return this->databaseInformation.endTime;
}

double NetworkInformation::getSimulationDuration() {
    return (this->databaseInformation.endTime
            - this->databaseInformation.startTime);
}

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

std::string NetworkInformation::removeSpaces(std::string s) {
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    return s;
}

std::vector<std::string> NetworkInformation::splitString(std::string s,
        const std::string& delimiter) {
    std::vector<std::string> tokens;
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

NetworkInformation* NetworkInformation::getInformation(
        omnetpp::cModule* owner) {
    if ((singleton == 0))
        singleton = new NetworkInformation(owner);
    return singleton;
}

/*void NetworkInformation::clearNetworkInformation() {
 if (singleton == 0)
 return;
 delete singleton;
 }*/

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

void NetworkInformation::setupAllRequests() {

    std::cout << "setting up all Requests, may take some time" << endl;
    const clock_t begin_time = clock();
    std::string query = "";
    std::string all = "ALL";
    if (std::strcmp(databaseInformation.category.c_str(), all.c_str()) == 0) {
        query = "SELECT * FROM " + databaseInformation.name + "."
                + databaseInformation.table + " WHERE requestTime BETWEEN "
                + boost::lexical_cast<std::string>(
                        databaseInformation.startTime) + " AND "
                + boost::lexical_cast<std::string>(databaseInformation.endTime)
                + " ORDER BY requestTime DESC;";
        std::cout << query;
    } else {
        query = "SELECT * FROM " + databaseInformation.name + "."
                + databaseInformation.table + " WHERE category =  \""
                + boost::lexical_cast<std::string>(databaseInformation.category)
                + "\" AND requestTime BETWEEN "
                + boost::lexical_cast<std::string>(
                        databaseInformation.startTime) + " AND "
                + boost::lexical_cast<std::string>(databaseInformation.endTime)
                + " ORDER BY requestTime DESC;";
        std::cout << query;
    }
    DBCursor* dbCursor = dataBaseConnection->selectAllRequests(query);
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

std::vector<std::vector<std::string>*>* NetworkInformation::getRequestsForClient(
        int id) {
    return this->allRequests[id];

}

DBConnection* NetworkInformation::getDatabaseConnection() {
    return this->dataBaseConnection;
}

void NetworkInformation::getStartTime() {
    std::string query = "SELECT MIN(requestTime) FROM "
            + databaseInformation.name + "." + databaseInformation.table + ";";
    DBCursor* dbCursor = dataBaseConnection->selectFirstTimestamp(query);
    std::vector<std::vector<std::string>*>* resultFristTimestamp =
            dbCursor->getResultSet();
    firstRequestTime = std::atof(resultFristTimestamp->at(0)->at(0).c_str());
    delete dbCursor;
    delete resultFristTimestamp->at(0);
    delete resultFristTimestamp;
}

void NetworkInformation::selectAllUsers() {
    std::string all = "ALL";
    std::string query = "";
    if (std::strcmp(databaseInformation.category.c_str(), all.c_str()) == 0) {
        query = "SELECT DISTINCT userId FROM " + databaseInformation.name + "."
                + databaseInformation.table + " WHERE requestTime BETWEEN "
                + boost::lexical_cast<std::string>(
                        databaseInformation.startTime) + " AND "
                + boost::lexical_cast<std::string>(databaseInformation.endTime)
                + ";";
    } else {
        query = "SELECT DISTINCT userId FROM " + databaseInformation.name + "."
                + databaseInformation.table + " WHERE category =  \""
                + boost::lexical_cast<std::string>(databaseInformation.category)
                + "\" AND requestTime BETWEEN "
                + boost::lexical_cast<std::string>(
                        databaseInformation.startTime) + " AND "
                + boost::lexical_cast<std::string>(databaseInformation.endTime)
                + ";";
    }

    DBCursor* dbCursor = dataBaseConnection->selectAllClients(query);
    std::vector<std::vector<std::string>*>* resultSet =
            dbCursor->getResultSet();
    userIds = resultSet->at(0);
    delete dbCursor;
    delete resultSet;
}

std::vector<std::string>* NetworkInformation::getClientIdsForReverseProxy(
        std::string proxyName) {
    return leafProxyVector[proxyName];
}

std::vector<ProxyCacheSettings_t>* NetworkInformation::getProxyCacheSettings() {
    return &proxyCacheSettings;
}

bool NetworkInformation::isConnected(int id, std::string proxy) {
    for (unsigned int i = 0; i < connectionTable[id]->size(); i++)
        if (proxyCacheSettings[connectionTable[id]->at(i)].name == proxy)
            return true;
    return false;
}

std::string NetworkInformation::getAdmissionStrategy(int id) {
    return proxyCacheSettings[id - 1].admissionStrategy;
}

std::string NetworkInformation::getEvictionStrategy(int id) {
    return proxyCacheSettings[id - 1].evictionStrategy;
}

void NetworkInformation::generateUserIdsPerProxy() {
    unsigned int share = amountOfClients / getAmountOfLeafProxys();
    unsigned int ueberhang = amountOfClients % getAmountOfLeafProxys();
    //int j = 0;

    for (int i = 0; i <= getAmountOfLeafProxys(); ++i) {
        std::vector<std::string>* newVec = new std::vector<std::string>();
        userIdsPerProxy->push_back(newVec);
    }
    srand(time(NULL));
    for (auto id : *userIds) {
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

void NetworkInformation::generateProxyCacheSettings() {
    for (auto line : config) {
        if (line[0] == 'R') {
            //std::string valueString = splitString(line, ":").at(1);
            std::vector<std::string> values = splitString(line, ";");
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

std::vector<std::pair<double, double>>* NetworkInformation::generateVectorOfDoublePairs(
        std::vector<std::string> values) {
    std::vector<std::pair<double, double>>* storageAlterationVector =
            new std::vector<std::pair<double, double>>();
    if (!(values.at(values.size() - 1) == "")) {
        for (auto i : values) {
            std::vector<std::string> value = splitString(removeSpaces(i), ":");
            storageAlterationVector->emplace_back(
                    std::make_pair(std::stod(value.at(0)),
                            std::stod(value.at(1))));
        }
    }

    return storageAlterationVector;
}

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
        std::vector<std::string> column = splitString(config[i], " ");
        column.erase(column.begin());
        std::vector<int>* connections = new std::vector<int>();
        for (unsigned int j = 0; j < column.size(); j++) {
            if (column[j] == "+")
                connections->push_back(j);
        }
        connectionTable[i] = connections;
    }
}

bool NetworkInformation::toBool(std::string s) {
    if (s == "true")
        return true;
    return false;
}

int NetworkInformation::getAmountOfLeafProxys() {
    int amountOfLeafProxys = 0;
    for (auto i : proxyCacheSettings)
        if (i.leaf)
            amountOfLeafProxys++;
    return amountOfLeafProxys;
}

std::map<int, std::vector<int>*>* NetworkInformation::getConnectionTable() {
    return &connectionTable;
}

std::map<std::string, std::vector<std::string>*>* NetworkInformation::getLeafProxyVector() {
    return &leafProxyVector;
}

int NetworkInformation::getAmountOfReverseProxys() {
    return this->amountOfReverseProxys;
}

int NetworkInformation::getAmountOfClients() {
    return this->amountOfClients;
}

int NetworkInformation::getGateSize(int id) {
    return connectionTable[id]->size();
}

double NetworkInformation::getFirstRequestTimed() {
    return this->firstRequestTime;
}

void NetworkInformation::increaseHit() {
    this->hits = hits + 1;
}

int NetworkInformation::getHits() {
    return this->hits;
}

int NetworkInformation::getAmountOfLevels() {
    return this->amountOfLevels;
}

int NetworkInformation::getCDNDelay() {
    return this->cdnDelay;
}

void NetworkInformation::readFile() {
    std::ifstream infile(path);
    std::string line;
    while (getline(infile, line)) {
        config.push_back(line);
    }
}

ProxyCacheSettings_t* NetworkInformation::getSettings(int id) {
    return &proxyCacheSettings.at(id - 1);
}
