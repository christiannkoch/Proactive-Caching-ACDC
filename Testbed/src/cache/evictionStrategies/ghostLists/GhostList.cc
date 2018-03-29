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
/* @file GhostList.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the GhostList
 *
 * @section DESCRIPTION
 */
#include <ghostLists/GhostList.h>
/*
 * @brief returns an instance of a GhostList
 * @return an instance of a GhostList
 */
GhostList::GhostList() {

}

GhostList::~GhostList() {
}

/*
 * @brief returns if a video id is in the ghost list
 * @param id the video id
 * @return true if contained in the ghost list, false otherwise
 */
bool GhostList::contains(std::string id) {
    if (container.find(id) == container.end())
        return false;
    else
        return true;
}
/*
 * @brief periodicEvents of the ghost list
 */
void GhostList::periodicEvents() {
}

/*
 * @brief inserts elements into the ghost list
 * @param id the list if video ids to insert
 */
void GhostList::insert(std::list<std::string>* id) {
    for (std::list<std::string>::const_iterator i = id->begin(); i != id->end();
            i++) {
        container[*i] = *i;
    }

    delete id;
}

/*
 * @brief deletes an entry from the ghost list
 * @param id the entry to delete
 */
void GhostList::deleteEntry(std::string id) {
    if (container.find(id) != container.end())
        container.erase(id);
}

/*
 * @brief returns the size of the ghost list
 */
long long GhostList::getSize() {
    return container.size();
}
