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
/* @file DatabaseFactory.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief a facotry for the desired database connection
 *
 * @section DESCRIPTION
 */
#include "DatabaseFactory.h"
#include "DBConnection.h"
#include "MySqlConnection.h"
#include <omnetpp.h>
#include <string>

using namespace sql;
using namespace omnetpp;

/*
 * @brief returns a connection to the database based upon the type of the database
 * @param type the type of the database, eg. mysql, nosql etc.
 * @param username the username of someone who can connect to the database
 * @param password the password corresponding to the user who wants to connect to the database
 * @param server the server address
 * @param port the port on which the database listenes to
 * @return a connection to a database based upon the type of the database
 */
DBConnection* DatabaseFactory::getConnection(const std::string type,
        const std::string username, const std::string password,
        const std::string server, int port) {
    if (type == "mysql")
        return new MySqlConnection(username, password, server, port);
    else
        throw "This type of Database does not exist, please choose a different one or implement it";
}

