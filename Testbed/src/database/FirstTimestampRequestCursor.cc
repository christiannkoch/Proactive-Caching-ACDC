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

#include <FirstTimestampRequestCursor.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <vector>
#include <string>

FirstTimestampRequestCursor::FirstTimestampRequestCursor(const std::string query,
        sql::Connection* connection) {
    // TODO Auto-generated constructor stub
    this->query = query;
    this->connection = connection;

}

FirstTimestampRequestCursor::~FirstTimestampRequestCursor() {
    // TODO Auto-generated destructor stub
}

void FirstTimestampRequestCursor::execute() {
    this->stmt = connection->createStatement();
    this->res = stmt->executeQuery(this->query);
    std::vector<std::string>* result = new std::vector<std::string>();
    res->next();
    result->push_back(std::to_string(res->getDouble(1)));
    delete res;
    delete stmt;
    resultSet->push_back(result);
}

std::vector<std::vector<std::string>*>* FirstTimestampRequestCursor::getResultSet() {
    return this->resultSet;
}
