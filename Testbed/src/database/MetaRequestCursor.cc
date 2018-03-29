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
/* @file MetaRequestCursor.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief returns the metadata for a video
 *
 * @section DESCRIPTION
 */

#include "MetaRequestCursor.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <vector>
#include <string>

/*
 * not needed
 */
MetaRequestCursor::MetaRequestCursor(const std::string query,
        sql::Connection* connection) {
    this->query = query;
    this->connection = connection;

}

MetaRequestCursor::~MetaRequestCursor() {
    delete stmt;
    delete res;
    delete connection;
}

/// Executes the given query.
/** This Function takes the query given at initialisation and executes it. It does not
 * check what type of query it is so be careful. */
void MetaRequestCursor::execute() {
    this->stmt = connection->createStatement();
    this->res = stmt->executeQuery(this->query);
    res->next();
    //this->videoid = string(res->getString(1));
    //this->duration = string(res->getString("duration"));
    //this->uploaded = string(res->getString("uploaded"));
    //this->uploaededDatetime = string(res->getString("uploaded_datetime"));
    //this->uploader = string(res->getString("uploader"));
    //this->category = string(res->getString("category"));
    //this->requested = res->getInt("requested");
    std::vector<std::string>* result = new std::vector<std::string>();
    result->push_back(std::string(res->getString(1)));
    result->push_back(std::string(res->getString("duration")));
    result->push_back(std::string(res->getString("uploaded")));
    result->push_back(std::string(res->getString("uploaded_datetime")));
    result->push_back(std::string(res->getString("uploader")));
    result->push_back(std::string(res->getString("category")));
    resultSet->push_back(result);
}

std::vector<std::vector<std::string>*>* MetaRequestCursor::getResultSet() {
    return this->resultSet;
}
