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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_NODES_RECENCYNODE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_NODES_RECENCYNODE_H_
#include <string>

class RecencyNode {
public:
    RecencyNode(std::string value, RecencyNode* prev, RecencyNode* next);
    virtual ~RecencyNode();
    void setPrev(RecencyNode* prev);
    void setNext(RecencyNode* next);
    RecencyNode* getPrev();
    RecencyNode* getNext();
    std::string getValue();
private:
    std::string value;
    RecencyNode* prev;
    RecencyNode* next;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_NODES_RECENCYNODE_H_ */
