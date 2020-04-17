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
/* @file RecencyNode.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements RecencyNodes
 *
 * @section DESCRIPTION
 *
 * Recency nodes are used in LRU and LRU-like implementations of eviction strategies.
 * They are used for finding the least recently used element fast.
 */
#include "RecencyNode.h"
/*
 * @brief returns an instance of a RecencyNode
 * @param value the value of the recency node which is the video id
 * @param prev the previous recency node
 * @param next the next recency node
 */
RecencyNode::RecencyNode(std::string value, RecencyNode* prev,
        RecencyNode* next) {
    this->value = value;
    this->prev = prev;
    this->next = next;
}

RecencyNode::~RecencyNode() {
}

/*
 * @brief sets the previous pointer
 * @param prev the previous recency node
 */
void RecencyNode::setPrev(RecencyNode* prev) {
    this->prev = prev;
}

/*
 * @brief sets the next pointer
 * @param next the next recency node
 */
void RecencyNode::setNext(RecencyNode* next) {
    this->next = next;
}

/*
 * @brief returns the previous recency node
 * @return the previous recency node
 */
RecencyNode* RecencyNode::getPrev() {
    return this->prev;
}

/*
 * @brief returns the next recency node
 * @return the next recency node
 */
RecencyNode* RecencyNode::getNext() {
    return this->next;
}

/*
 * @brief returns the value of the recency node
 * @return the value of the recency node
 */
std::string RecencyNode::getValue() {
    return this->value;
}
