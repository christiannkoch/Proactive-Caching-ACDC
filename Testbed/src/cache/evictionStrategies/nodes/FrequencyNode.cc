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

#include <nodes/FrequencyNode.h>
#include <string>
#include <set>

FrequencyNode::FrequencyNode(int value, std::set<std::string>* items,
        FrequencyNode* prev, FrequencyNode* next) {
    this->value = value;
    this->items = items;
    this->prev = prev;
    this->next = next;

}

FrequencyNode::~FrequencyNode() {
    delete items;
}

void FrequencyNode::setPrev(FrequencyNode* prev) {
    this->prev = prev;
}

void FrequencyNode::setNext(FrequencyNode* next) {
    this->next = next;
}

FrequencyNode* FrequencyNode::getPrev() {
    return this->prev;
}

FrequencyNode* FrequencyNode::getNext() {
    return this->next;
}

int FrequencyNode::getValue() {
    return this->value;
}

std::set<std::string>* FrequencyNode::getItems() {
    return this->items;
}
