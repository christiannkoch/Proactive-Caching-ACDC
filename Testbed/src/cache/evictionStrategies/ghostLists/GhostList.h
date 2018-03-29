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
/* @file GhostList.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for the GhostList
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_EVICTIONSTRATEGIES_GHOSTLISTS_GHOSTLIST_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_GHOSTLISTS_GHOSTLIST_H_

#include <map>
#include <string>
#include "BasicGhostList.h"

class GhostList : public BasicGhostList{
public:
    GhostList();
    virtual ~GhostList();
    bool contains(std::string id);
    void insert(std::list<std::string>* id);
    void periodicEvents();
    void deleteEntry(std::string id);
    long long getSize();
protected:
    std::map<std::string,std::string> container;

};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_GHOSTLISTS_GHOSTLIST_H_ */
