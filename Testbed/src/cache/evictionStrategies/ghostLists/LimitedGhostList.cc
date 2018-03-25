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

#include <ghostLists/LimitedGhostList.h>
#include <nodes/RecencyNode.h>
#include <map>
#include <string>

LimitedGhostList::LimitedGhostList(long long size) {
    // TODO Auto-generated constructor stub
    this->size = size;
    this->head = new RecencyNode("0", nullptr, nullptr);
    head->setNext(head);
    head->setPrev(head);
}

LimitedGhostList::~LimitedGhostList() {
    // TODO Auto-generated destructor stub
    RecencyNode* rec = head->getNext();
    while (rec->getValue() != "0") {
        rec = rec->getNext();
        delete rec->getPrev();
    }

    delete head;
}

void LimitedGhostList::deleteEntry(std::string id) {
    if (container.find(id) != container.end()) {
        RecencyNode* rec = container[id];
        rec->getPrev()->setNext(rec->getNext());
        rec->getNext()->setPrev(rec->getPrev());
        delete rec;
        container.erase(id);
    }

}

bool LimitedGhostList::contains(std::string id) {
    if (container.find(id) == container.end())
        return false;
    else {
        rearrangeGhostList(id);
        return true;
    }

}

void LimitedGhostList::insert(std::list<std::string>* id) {
    if (container.size() > size - 1) {
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

void LimitedGhostList::periodicEvents() {

}

long long LimitedGhostList::getSize() {
    return this->size;
}
void LimitedGhostList::rearrangeGhostList(std::string id) {
    RecencyNode* rec = container[id];
    RecencyNode* next_rec = rec->getNext();
    rec->getPrev()->setNext(next_rec);
    next_rec->setPrev(rec->getPrev());
    rec->setNext(head->getNext());
    rec->getNext()->setPrev(rec);
    rec->setPrev(head);
    head->setNext(rec);
}
