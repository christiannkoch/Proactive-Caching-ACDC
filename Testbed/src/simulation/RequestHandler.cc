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
/* @file RequestHandler.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the RequestHandler
 *
 * @section DESCRIPTION
 * The request handler handles requests that arrive in a time where a request for the same
 * video segment is already forwareded and stores this request to not send another request for the same
 * video segment.
 */
#include "RequestHandler.h"
#include <string>
#include <vector>
#include <map>
#include <omnetpp.h>

RequestHandler::RequestHandler() {

}

RequestHandler::~RequestHandler() {
}

/*
 * @brief inserts a request into a queue of requests
 *
 * @return true if a queue for this requested video id already existed, false of not
 */
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

/*
 * @brief returns a list of the gates to which to send the video segment to
 *
 * @param id the video id
 *
 * @return a list of gates to which the video segment has to be sent to
 */
std::vector<std::pair<int, double>*>* RequestHandler::getRequestList(
        const std::string id) {
    if (container.find(id) == container.end())
            return nullptr;
        else
            return container.at(id);

}

/*
 * @brief returns true if a queue for a video id already exists
 *
 * @return true if a queue for a video already exists, false otherwise
 */
bool RequestHandler::contains(std::string id) {
    if (container.find(id) == container.end())
        return false;
    else
        return true;
}

/*
 * @brief deletes the queue of a given video id
 *
 * @param id the id of a video
 */
void RequestHandler::deleteList(std::string id) {
    for (std::vector<std::pair<int, double>*>::iterator iter =
            container.at(id)->begin(); iter != container.at(id)->end();
            iter++) {
        delete *iter;
    }

    delete container.at(id);
    container.erase(id);
}
