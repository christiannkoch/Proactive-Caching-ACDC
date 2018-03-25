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

#include <nodes/RecencyNode.h>

RecencyNode::RecencyNode(std::string value, RecencyNode* prev,
        RecencyNode* next) {
    // TODO Auto-generated constructor stub
    this->value = value;
    this->prev = prev;
    this->next = next;
}

RecencyNode::~RecencyNode() {
    // TODO Auto-generated destructor stub
}

void RecencyNode::setPrev(RecencyNode* prev) {
    this->prev = prev;
}

void RecencyNode::setNext(RecencyNode* next) {
    this->next = next;
}

RecencyNode* RecencyNode::getPrev() {
    return this->prev;
}

RecencyNode* RecencyNode::getNext() {
    return this->next;
}

std::string RecencyNode::getValue() {
    return this->value;
}
