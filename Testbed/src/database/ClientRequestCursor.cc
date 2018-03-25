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

#include "ClientRequestCursor.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <vector>
#include <string>

ClientRequestCursor::ClientRequestCursor(const std::string query, sql::Connection* connection) {
    // TODO Auto-generated constructor stub
this->query = query;
this->connection = connection;
}

ClientRequestCursor::~ClientRequestCursor() {
    // TODO Auto-generated destructor stub
}

void ClientRequestCursor::execute(){
    this->stmt = connection->createStatement();
    this->res = stmt->executeQuery(this->query);
    while(res->next()){
        std::vector<std::string>* result = new std::vector<std::string>();
        result->push_back(std::string(res->getString(1)));
        result->push_back(std::to_string(res->getDouble(2)));
        result->push_back(std::to_string(res->getInt64(3)));
        result->push_back(std::string(res->getString(4)));
        result->push_back(std::to_string(res->getDouble(5)));
        result->push_back(std::to_string(res->getDouble(6)));
        result->push_back(std::to_string(res->getInt64(7)));
        result->push_back(std::string(res->getString(8)));
        result->push_back(std::string(res->getString(9)));
        result->push_back(std::string(res->getString(10)));
        result->push_back(std::string(res->getString(11)));
        result->push_back(std::to_string(res->getInt(12)));
        resultSet->push_back(result);
    }
    delete res;
    delete stmt;
}

std::vector<std::vector<std::string>*>* ClientRequestCursor::getResultSet() {
    return this->resultSet;
}
