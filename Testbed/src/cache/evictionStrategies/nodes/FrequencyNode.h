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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_NODES_FREQUENCYNODE_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_NODES_FREQUENCYNODE_H_
#include <string>
#include <set>

class FrequencyNode {
public:
    FrequencyNode(int value, std::set<std::string>* items, FrequencyNode* prev,
            FrequencyNode* next);
    virtual ~FrequencyNode();
    void setPrev(FrequencyNode* prev);
    void setNext(FrequencyNode* next);
    FrequencyNode* getPrev();
    FrequencyNode* getNext();
    int getValue();
    std::set<std::string>* getItems();
private:
    int value;
    std::set<std::string>* items;
    FrequencyNode* prev;
    FrequencyNode* next;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_NODES_FREQUENCYNODE_H_ */
