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
/* @file AllClientsRequestCursor.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief returns all unique clients
 *
 * @section DESCRIPTION
 */
#include "AllClientsRequestCursor.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <vector>
#include <string>

/*
 * @brief returns an instance of the AllClientsRequestCursor
 * @param query the query which is executed on the database
 * @param connection the connection to the database
 */
AllClientsRequestCursor::AllClientsRequestCursor(const std::string query,
        sql::Connection* connection) {
    this->query = query;
    this->connection = connection;
}

AllClientsRequestCursor::~AllClientsRequestCursor() {
}

/*
 * @brief executes a command on the database and saves the result
 *
 */
void AllClientsRequestCursor::execute() {
    this->stmt = connection->createStatement();
    this->res = stmt->executeQuery(this->query);
    std::vector<std::string>* result = new std::vector<std::string>();
    while (res->next()) {
        result->push_back(std::to_string(res->getInt64(1)));
    }
    resultSet->push_back(result);
    delete stmt;
    delete res;
}
/*
 * @brief returns the result of the statement executed on the database
 * @return the result from the statement execution on the database
 */
std::vector<std::vector<std::string>*>* AllClientsRequestCursor::getResultSet() {
    return this->resultSet;
}
