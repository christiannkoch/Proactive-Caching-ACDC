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
/* @file DBConnection.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the DBConnection
 *
 * @section DESCRIPTION
 */
#ifndef SRC_DATABASE_DBCONNECTION_H_
#define SRC_DATABASE_DBCONNECTION_H_
#include "DBCursor.h"
#include <string>
#include "../simulation/DatabaseInformation.h"

class DBConnection {
public:
    DBConnection();
    virtual ~DBConnection();
    virtual bool connect()=0;
    virtual void close() = 0;
    virtual DBCursor* selectMetaData(DatabseInformation_t databaseInformation) = 0;
    virtual DBCursor* selectAllRequests(DatabseInformation_t databaseInformation) = 0;
    virtual DBCursor* selectAllClients(DatabseInformation_t databaseInformation) = 0;
    virtual DBCursor* selectFirstTimestamp(DatabseInformation_t databaseInformation)= 0;
protected:
    std::string getUsername();
    std::string getPassword();
    std::string getServer();
    int getPort();
private:
    std::string username;
    std::string password;
    std::string server;
    int port;
};

#endif /* SRC_DATABASE_DBCONNECTION_H_ */
