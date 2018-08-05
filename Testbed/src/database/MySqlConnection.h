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
/* @file MySqlConnection.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the MySqlConnection
 *
 * @section DESCRIPTION
 */
#ifndef SRC_DATABASE_MYSQLCONNECTION_H_
#define SRC_DATABASE_MYSQLCONNECTION_H_

#include <mysql_connection.h>
#include <iostream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <DBCursor.h>
#include <string>
#include "DBConnection.h"
#include "DatabaseInformation.h"

class MySqlConnection : public DBConnection {
public:
    MySqlConnection(std::string username, std::string password, std::string server,std::string dbName, int port);
    virtual ~MySqlConnection();
    bool connect();
    void close();
    DBCursor* selectMetaData(DatabseInformation_t databaseInformation);
    DBCursor* selectAllRequests(DatabseInformation_t databaseInformation);
    DBCursor* selectAllClients(DatabseInformation_t databaseInformation);
    DBCursor* selectFirstTimestamp(DatabseInformation_t databaseInformation);
protected:
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    std::string getUsername();
    std::string getPassword();
    std::string getServer();
    int getPort();
    double startTime;
    double endTime;
private:
    std::string username;
    std::string password;
    std::string server;
    int port;
    DBCursor* dbCursor;
    std::string dbName;
};

#endif /* SRC_DATABASE_MYSQLCONNECTION_H_ */
