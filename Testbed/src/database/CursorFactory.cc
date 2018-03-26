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

#include "CursorFactory.h"
#include <cppconn/driver.h>
#include "MetaRequestCursor.h"
#include "ClientRequestCursor.h"
#include "AllClientsRequestCursor.h"
#include "FirstTimestampRequestCursor.h"

CursorFactory::CursorFactory() {

}

/*
 * @brief returns a cursor based upon the execution we want to make
 * @param query the query we want to execute
 * @param connection the connection to the database
 * @param type the type of the cursor
 * @return an instance of the cursor we need to execute
 */

DBCursor* CursorFactory::createCursor(const std::string query,
        sql::Connection* connection, const std::string type) {
    if (type == "meta")
        return new MetaRequestCursor(query, connection);
    if (type == "request")
        return new ClientRequestCursor(query, connection);
    if (type == "allClients")
        return new AllClientsRequestCursor(query, connection);
    if (type == "firstTimestamp")
        return new FirstTimestampRequestCursor(query, connection);
    else
        throw "this type of Cursor does not exist\n";
}

