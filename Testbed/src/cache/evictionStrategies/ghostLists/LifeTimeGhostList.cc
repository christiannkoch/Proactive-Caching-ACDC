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

#include <ghostLists/LifeTimeGhostList.h>
#include <omnetpp.h>
#include <string>
#include <map>
LifeTimeGhostList::LifeTimeGhostList() {
    // TODO Auto-generated constructor stub
    this->lifeTime = 14400;
}

LifeTimeGhostList::~LifeTimeGhostList() {
    // TODO Auto-generated destructor stub
}

bool LifeTimeGhostList::contains(std::string id) {
    if (container.find(id) == container.end())
        return false;
    else
        return true;
}

void LifeTimeGhostList::periodicEvents() {
    for (auto i = container.begin(); i != container.end(); i++) {
        std::string toDelete = (*i).first;
        if ((omnetpp::simTime().dbl() - container[toDelete]) > lifeTime) {
            container.erase(toDelete);
        }
    }
}
void LifeTimeGhostList::insert(std::list<std::string>* id) {
    for (std::list<std::string>::const_iterator i = id->begin(); i != id->end();
            i++) {
        container[*i] = omnetpp::simTime().dbl();
    }

    delete id;
}

void LifeTimeGhostList::deleteEntry(std::string id) {
    if (container.find(id) != container.end())
        container.erase(id);
}

long long LifeTimeGhostList::getSize() {
    return container.size();
}
