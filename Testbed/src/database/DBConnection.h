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

#ifndef SRC_DATABASE_DBCONNECTION_H_
#define SRC_DATABASE_DBCONNECTION_H_
#include "DBCursor.h"
#include <string>

class DBConnection {
public:
    DBConnection();
    virtual ~DBConnection();
    virtual bool connect()=0;
    virtual void close() = 0;
    virtual DBCursor* selectMetaData(const std::string query) = 0;
    virtual DBCursor* selectAllRequests(const std::string query) = 0;
    virtual DBCursor* selectAllClients(const std::string query) = 0;
    virtual DBCursor* selectFirstTimestamp(const std::string query)= 0;
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
