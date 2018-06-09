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
/* @file MySqlConnection.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief returns a connection to a mysql database
 *
 * @section DESCRIPTION
 */
#include "MySqlConnection.h"
#include <stdlib.h>
#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <omnetpp.h>
#include <string>
#include "MetaRequestCursor.h"
#include "CursorFactory.h"

using namespace omnetpp;

/**
 * @brief the constructor for the MySQL connection
 *
 * The MySQL Connection is needed to run querries on the database we use.
 * It stores all the information needed to connect to the database at any given time.
 *
 * @param username the login username for accessing the database
 *
 * @param password the password for accessing the database
 *
 * @param server the address of the server. if running on a local machine it is localhost
 *
 * @param port the port to connect to
 *
 * @return a pointer to the MySQL Connection
 */
MySqlConnection::MySqlConnection(std::string username, std::string password,
        std::string server, std::string dbName,int port) {
    this->username = username;
    this->password = password;
    this->server = server;
    this->port = port;
    this->startTime = startTime;
    this->endTime= endTime;
    this->dbName = dbName;
    // TODO Auto-generated constructor stub

}

MySqlConnection::~MySqlConnection() {
    // TODO Auto-generated destructor stub
}

/**
 * @brief connects to the database
 *
 * We do not want to stay connected to the database all the time. With this function we connect to the database.
 * If it was successfull we return true. Otherwise false.
 *
 * @return a boolean value indicating if the connection was successfull
 */
bool MySqlConnection::connect() {
    driver = get_driver_instance();
    std::string serverAddressPort = static_cast<std::ostringstream*>(&(std::ostringstream()
            << getPort()))->str();
    std::string serveradress = getServer() + serverAddressPort;
    con = driver->connect(serveradress, getUsername(), getPassword());
    stmt = con->createStatement();
    stmt->execute("USE "+dbName+";");
    delete stmt;
    if (con->isValid())
        return true;
    else
        return false;

}

/**
 * @brief closes the connection to the database
 */
void MySqlConnection::close() {
    con->close();
    delete con;
}

/**
 * @brief creates the cursor that selects the metadata and executes it
 *
 * @param query a string that is a mysql statement which upon execution results in fetching the metadata
 *
 * @return a pointer to the cursor that selects the metadata
 */
DBCursor* MySqlConnection::selectMetaData(const std::string query) {
    connect();
    try {
        dbCursor = CursorFactory::createCursor(query, this->con,
                "meta");
    } catch (char const* msg) {
        EV << msg;
    }
    dbCursor->execute();
    close();
    return dbCursor;
}

/**
 * @brief creates the cursor that selects all requests and executes it
 *
 * @param query a string that is a mysql statement which upon execution results in fetching all requests
 *
 * @return a pointer to the cursor that selects all requests
 */

DBCursor* MySqlConnection::selectAllRequests(const std::string query) {
    connect();
    try {
        dbCursor = CursorFactory::createCursor(query, this->con,
                "request");
    } catch (char const* msg) {
        EV << msg;
    }
    dbCursor->execute();
    close();
    return dbCursor;
}

/**
 * @brief creates the cursor that selects all unique clients and executes it
 *
 * @param query a string that is a mysql statement which upon execution results in fetching all unique clients
 *
 * @return a pointer to the cursor that selects all unique clients
 */
DBCursor* MySqlConnection::selectAllClients(const std::string query) {
    connect();
    try {
        dbCursor = CursorFactory::createCursor(query, this->con,
                "allClients");
    } catch (char const* msg) {
        EV << msg;
    }
    dbCursor->execute();
    close();
    return dbCursor;
}

/**
 * @brief creates the cursor that selects the time of the first request in the dataset and executes it
 *
 * @param query a string that is a mysql statement which upon execution results in fetching the time of the first request in the dataset
 *
 * @return a pointer to the cursor that selects the time of the first request in the dataset
 */
DBCursor* MySqlConnection::selectFirstTimestamp(const std::string query) {

    connect();
    try {
        dbCursor = CursorFactory::createCursor(query, this->con,
                "firstTimestamp");
    } catch (char const* msg) {
        EV << msg;
    }
    dbCursor->execute();
    close();
    return dbCursor;
}

/**
 * @brief returns the name of the user accessing the database
 *
 * @return the name of the user accessing the database
 */

std::string MySqlConnection::getUsername() {
    return this->username;
}

/**
 * @brief returns the password of the user accessing the database
 *
 * @return the password of the user accessing the database
 */
std::string MySqlConnection::getPassword() {
    return this->password;
}

/**
 * @brief returns the server address of the database to access
 *
 * @return the server address of the database
 */
std::string MySqlConnection::getServer() {
    return this->server;
}

/**
 * @brief returns the port of the database to access
 *
 * @return the port of the database to access
 */
int MySqlConnection::getPort() {
    return this->port;
}
