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
/* @file RequestHandler.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the RequestHandler
 *
 * @section DESCRIPTION
 */
#ifndef SRC_SIMULATION_REQUESTHANDLER_H_
#define SRC_SIMULATION_REQUESTHANDLER_H_
#include <string>
#include <vector>
#include <map>
#include <omnetpp.h>

class RequestHandler {
public:
    RequestHandler();
    virtual ~RequestHandler();
    bool insertRequest(std::string id, int gate);
    std::vector<std::pair<int,double>*>* getRequestList(std::string id);
    void deleteList(std::string id);
private:
    bool contains(std::string id);
    std::map<std::string, std::vector<std::pair<int,double>*>*> container;
};

#endif /* SRC_SIMULATION_REQUESTHANDLER_H_ */
