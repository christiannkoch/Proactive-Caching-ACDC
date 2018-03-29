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
/* @file NHIT.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief header file for NHIT
 *
 * @section DESCRIPTION
 */
#ifndef SRC_CACHE_ADMISSIONSTRETEGIES_NHIT_NHIT_H_
#define SRC_CACHE_ADMISSIONSTRETEGIES_NHIT_NHIT_H_
#include "BasicAdmissionStrategy.h"
#include <vector>
#include <map>

class NHIT :  public BasicAdmissionStrategy{
public:
    NHIT(std::vector<std::string>* parameters);
    virtual ~NHIT();
    bool toBeCached(VideoSegment* pkg);
    void periodicEvents();
private:
    std::map<std::string, std::vector<double>*> seenPackets;
    unsigned int thresholdValue;
    double timeFrame;
    std::vector<std::string> stringsToDelete;
};

#endif /* SRC_CACHE_ADMISSIONSTRETEGIES_NHIT_NHIT_H_ */
