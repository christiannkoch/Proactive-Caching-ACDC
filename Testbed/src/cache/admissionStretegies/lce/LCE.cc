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
/* @file LCE.cc
 * @author Johannes Pfannmüller
 * @date
 * @version 1.0
 *
 * @brief Implements Leave Copy Everywhere Strategy
 *
 * @section DESCRIPTION
 *
 * This is a Class responsible for implementing the behaviour of the Leave Copy Everywhere Strategy.
 * The Leave Copy Everywhere Strategy signals any cache for a video segment that passes that cache to
 * store a copy of this segment in its storage.
 *
 */

#include "LCE.h"

LCE::LCE() {
    // TODO Auto-generated constructor stub

}

LCE::~LCE() {
    // TODO Auto-generated destructor stub
}

/**
 * @brief decides if the copy of a given video segment will be stored
 *
 * @param pkg is a pointer to an initialized VideoSegment
 * @return boolean always true signaling to store the video segment.
 */
bool LCE::toBeCached(VideoSegment* pkg){
    return true;
}


/**
 * @brief periodic events
 *
 * @return void
 */
void LCE::periodicEvents(){

}
