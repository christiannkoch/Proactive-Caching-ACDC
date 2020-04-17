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
/* @file LCE.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for LCE
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_ADMISSIONSTRETEGIES_LCE_LCE_H_
#define SRC_CACHE_ADMISSIONSTRETEGIES_LCE_LCE_H_
#include "../BasicAdmissionStrategy.h"

class LCE : public BasicAdmissionStrategy{
public:
    LCE();
    virtual ~LCE();
    bool toBeCached(VideoSegment* pkg);
    void periodicEvents();
};

#endif /* SRC_CACHE_ADMISSIONSTRETEGIES_LCE_LCE_H_ */
