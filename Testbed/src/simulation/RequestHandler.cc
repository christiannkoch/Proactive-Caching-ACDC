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
#include "RequestHandler.h"
#include <string>
#include <vector>
#include <map>
#include <omnetpp.h>

RequestHandler::RequestHandler() {
    // TODO Auto-generated constructor stub

}

RequestHandler::~RequestHandler() {
}

bool RequestHandler::insertRequest(const std::string id, int gate) {
    if (contains(id)) {
        std::pair<int, double>* newPair = new std::pair<int, double>(gate,
                omnetpp::simTime().dbl());
        container.at(id)->push_back(newPair);
        return false;
    } else {
        std::vector<std::pair<int, double>*>* temp = new std::vector<
                std::pair<int, double>*>();
        temp->push_back(
                new std::pair<int, double>(gate, omnetpp::simTime().dbl()));
        container[id] = temp;
        return true;
    }
}

std::vector<std::pair<int, double>*>* RequestHandler::getRequestList(
        const std::string id) {
    if (container.find(id) == container.end())
            return nullptr;
        else
            return container.at(id);

}

bool RequestHandler::contains(std::string id) {
    if (container.find(id) == container.end())
        return false;
    else
        return true;
}

void RequestHandler::deleteList(std::string id) {
    for (std::vector<std::pair<int, double>*>::iterator iter =
            container.at(id)->begin(); iter != container.at(id)->end();
            iter++) {
        delete *iter;
    }

    delete container.at(id);
    container.erase(id);
}
