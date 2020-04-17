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
/* @file FrequencyNode.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements Frequency Nodes
 *
 * @section DESCRIPTION
 * Frequency nodes are used for implementing queues used by LFU to get the least frequently used
 * element fast.
 */
#include "FrequencyNode.h"
#include <string>
#include <set>
/*
 * @brief returns an instance of a frequency node
 * @param value the value of the frequency node. it indicates the frequency of accesses of the elements in it
 * @param items list of video ids that are accessed value times
 * @param prev the previous frequency node
 * @param next the next frequency node
 * @return an instance of a frequency node
 */
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
/*
 * @brief sets the previous pointer to the previous frequency node
 * @param prev the previous frequency node
 */
void FrequencyNode::setPrev(FrequencyNode* prev) {
    this->prev = prev;
}
/*
 * @brief sets the next pointer to the next frequency node
 * @param prev the next frequency node
 */
void FrequencyNode::setNext(FrequencyNode* next) {
    this->next = next;
}
/*
 * @brief returns the previous frequency node
 * @return the previous frequency node
 */
FrequencyNode* FrequencyNode::getPrev() {
    return this->prev;
}
/*
 * @brief returns the next frequency node
 * @return the next frequency node
 */
FrequencyNode* FrequencyNode::getNext() {
    return this->next;
}

/*
 * @brief returns the access frequency of the frequency node
 * @return the access frequency of the frequency node
 */
int FrequencyNode::getValue() {
    return this->value;
}

/*
 * @brief returns the list of items stored in the frequency node
 * @return the list of items stored in the frequency node
 */
std::set<std::string>* FrequencyNode::getItems() {
    return this->items;
}
