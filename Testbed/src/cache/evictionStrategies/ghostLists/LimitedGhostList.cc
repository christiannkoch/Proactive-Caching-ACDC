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
/* @file LimitedGhostList.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the LimitedGhostList
 *
 * @section DESCRIPTION
 */
#include "LimitedGhostList.h"
#include <map>
#include <string>

/*
 * @brief returns an instance of a LimitedGhostList
 *
 * @param size the maximum size of the ghostlist
 * the size limit of the ghost list is set to size
 * @return an instance of a LimitedGhostList
 */
LimitedGhostList::LimitedGhostList(long long size) {
    // TODO Auto-generated constructor stub
    this->size = size;
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
}

LimitedGhostList::~LimitedGhostList() {
    RecencyNode* rec = head->getNext();
    while (rec->getValue() != "0") {
        rec = rec->getNext();
        delete rec->getPrev();
    }

    delete head;
}
/*
 * @brief deletes an entry from the ghost list
 * @param id the entry to delete
 */
void LimitedGhostList::deleteEntry(std::string id) {
    if (container.find(id) != container.end()) {
        RecencyNode* rec = container[id];
        rec->getPrev()->setNext(rec->getNext());
        rec->getNext()->setPrev(rec->getPrev());
        delete rec;
        container.erase(id);
    }

}
/*
 * @brief returns if a video id is in the ghost list
 * @param id the video id
 * @return true if contained in the ghost list, false otherwise
 */
bool LimitedGhostList::contains(std::string id) {
    if (container.find(id) == container.end())
        return false;
    else {
        deleteEntry(id);
        return true;
    }

}
/*
 * @brief inserts elements into the ghost list
 * @param id the list if video ids to insert
 */
void LimitedGhostList::insert(std::list<std::string>* id) {
    while (container.size() + id->size() > size) {
        std::string toDelete = head->getPrev()->getValue();
        deleteEntry(toDelete);
    }
    for (std::list<std::string>::const_iterator i = id->begin(); i != id->end();
            i++) {
        container[*i] = new RecencyNode(*i, head, head->getNext());
        head->getNext()->setPrev(container[*i]);
        head->setNext(container[*i]);
    }
    delete id;

}
/*
 * @brief periodicEvents of the ghost list
 * deletes all elements that have expired
 */
void LimitedGhostList::periodicEvents() {

}
/*
 * @brief returns the size of the ghost list
 */
long long LimitedGhostList::getSize() {
    return this->size;
}
