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

#include "DatabaseFactory.h"
#include "DBConnection.h"
#include "MySqlConnection.h"
#include <omnetpp.h>
#include <string>

using namespace sql;
using namespace omnetpp;

DBConnection* DatabaseFactory::getConnection(const std::string type,
        const std::string username, const std::string password,
        const std::string server, int port) {
    if (type == "mysql")
        return new MySqlConnection(username, password, server, port);
    else
        throw "This type of Database does not exist, please choose a different one or implement it";
}

