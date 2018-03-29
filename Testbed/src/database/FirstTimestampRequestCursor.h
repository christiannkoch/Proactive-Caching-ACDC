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
/* @file FirstTimeStampRequestCursor.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements Move Copy Down Strategy
 *
 * @section DESCRIPTION
 */
#ifndef SRC_DATABASE_FIRSTTIMESTAMPREQUESTCURSOR_H_
#define SRC_DATABASE_FIRSTTIMESTAMPREQUESTCURSOR_H_
#include <string>
#include "DBCursor.h"
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class FirstTimestampRequestCursor : public DBCursor{
public:
    FirstTimestampRequestCursor(const std::string query,
            sql::Connection* connection);
    virtual ~FirstTimestampRequestCursor();
    void execute();
    std::vector<std::vector<std::string>*>* getResultSet();
protected:
    sql::Connection *connection;
    sql::Statement *stmt;
    sql::ResultSet *res;
    std::string query;
    std::vector<std::vector<std::string>*>* resultSet = new std::vector<std::vector<std::string>*>();
};

#endif /* SRC_DATABASE_FIRSTTIMESTAMPREQUESTCURSOR_H_ */
