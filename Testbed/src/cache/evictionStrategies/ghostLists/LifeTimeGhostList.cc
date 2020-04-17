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
/* @file LiftTimeGhostList.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the LifeTimeGhostList
 *
 * @section DESCRIPTION
 */
#include "LifeTimeGhostList.h"
#include <omnetpp.h>
#include <string>
#include <map>
/*
 * @brief returns an instance of a LifeTimeGhostList
 * the lifetime of the elements in the ghost list is set to 4 hours
 * @return an instance of a LifeTimeGhostList
 */
LifeTimeGhostList::LifeTimeGhostList() {
    this->lifeTime = 14400;
}

LifeTimeGhostList::~LifeTimeGhostList() {
}
/*
 * @brief returns if a video id is in the ghost list
 * @param id the video id
 * @return true if contained in the ghost list, false otherwise
 */
bool LifeTimeGhostList::contains(std::string id) {
    if (container.find(id) == container.end())
        return false;
    else
        return true;
}
/*
 * @brief periodicEvents of the ghost list
 * deletes all elements that have expired
 */
void LifeTimeGhostList::periodicEvents() {
    for (auto i = container.begin(); i != container.end(); i++) {
        std::string toDelete = (*i).first;
        if ((omnetpp::simTime().dbl() - container[toDelete]) > lifeTime) {
            container.erase(toDelete);
        }
    }
}

/*
 * @brief inserts elements into the ghost list
 * @param id the list if video ids to insert
 */
void LifeTimeGhostList::insert(std::list<std::string>* id) {
    for (std::list<std::string>::const_iterator i = id->begin(); i != id->end();
            i++) {
        container[*i] = omnetpp::simTime().dbl();
    }

    delete id;
}

/*
 * @brief deletes an entry from the ghost list
 * @param id the entry to delete
 */
void LifeTimeGhostList::deleteEntry(std::string id) {
    if (container.find(id) != container.end())
        container.erase(id);
}

/*
 * @brief returns the size of the ghost list
 */
long long LifeTimeGhostList::getSize() {
    return container.size();
}
