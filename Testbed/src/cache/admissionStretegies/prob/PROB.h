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
/* @file PROB.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for PROB
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_ADMISSIONSTRETEGIES_PROB_PROB_H_
#define SRC_CACHE_ADMISSIONSTRETEGIES_PROB_PROB_H_
#include "BasicAdmissionStrategy.h"

class PROB : public BasicAdmissionStrategy{
public:
    PROB(std::vector<std::string>* parameters);
    virtual ~PROB();
    bool toBeCached(VideoSegment* pkg);
    void periodicEvents();
private:
    double propability;
};

#endif /* SRC_CACHE_ADMISSIONSTRETEGIES_PROB_PROB_H_ */
