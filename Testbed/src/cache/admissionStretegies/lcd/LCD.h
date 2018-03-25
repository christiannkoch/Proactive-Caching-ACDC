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
 * @author Johannes Pfannmüller
 * @date
 * @version 1.0
 *
 * @brief Implements Leave Copy Down Strategy
 *
 * @section DESCRIPTION
 *
 * This is a Class responsible for implementing the behaviour of the Leave Copy Down Strategy.
 * Its main responsibility is to return a boolean value indicating if the video segment generated
 * a hit on the reverse proxy above in the topology and signaling the reverse proxy to store a copy
 * of this video segment.
 *
 */
#ifndef SRC_CACHE_ADMISSIONSTRETEGIES_LCD_LCD_H_
#define SRC_CACHE_ADMISSIONSTRETEGIES_LCD_LCD_H_
#include "BasicAdmissionStrategy.h"


/**
 * @brief Class that models the behaviour of the Leave Copy Down Admission Strategy
 */
class LCD : public BasicAdmissionStrategy{
public:
    // Default constructor
    LCD();

    // Destructor
    virtual ~LCD();

    // Answer true iff package has to be cached
    bool toBeCached(VideoSegment* pkg);

    // Perform periodic events
    void periodicEvents();
};

#endif /* SRC_CACHE_ADMISSIONSTRETEGIES_LCD_LCD_H_ */
