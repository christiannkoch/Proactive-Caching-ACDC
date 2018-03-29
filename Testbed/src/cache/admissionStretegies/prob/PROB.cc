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

/* @file LCD.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements Leave Copy Down Strategy
 *
 * @section DESCRIPTION
 *
 * This is a Class responsible for implementing the behaviour of the Probability Strategy.
 * It decides if the reverse proxy stores a copy of the video segment with a certain probability
 * by returning a boolean value.
 *
 */

#include "PROB.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

PROB::PROB(std::vector<std::string>* parameters) {
    // TODO Auto-generated constructor stub
    propability = std::stod(parameters->at(0));
}

PROB::~PROB() {
    // TODO Auto-generated destructor stub
}

/**
 * @brief decides if the copy of a given video segment will be stored
 *
 * A segment is cached with a certain probability
 * @param pkg is a pointer to an initialized VideoSegment
 * @return boolean signaling if a copy of the video segment should be stored
 */
bool PROB::toBeCached(VideoSegment* pkg) {
    srand(time(NULL));
    double random = rand() % 100; // zahl zwischen 1 und 2
    if (random <= propability * 100)
        return true;
    else
        return false;
}

/**
 * @brief periodic events
 *
 * @return void
 */
void PROB::periodicEvents() {

}
