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

#include "AdmissionStrategyFactory.h"
#include <string>
#include "NHIT.h"
#include "LCD.h"
#include "LCE.h"
#include "PROB.h"
#include "RandomAdmission.h"
#include "MCD.h"

AdmissionStrategyFactory::AdmissionStrategyFactory() {
    // TODO Auto-generated constructor stub

}
/** @brief creates the desired admissino strategy
 * @param admission a string representing the type of admission strategy
 * @param parameters a pointer to an optional vector of strings that can contain settings for the specific admission strategy
 * @return The specific admission strategy
 */
BasicAdmissionStrategy* AdmissionStrategyFactory::createAdmissionStrategy(
        std::string admission, std::vector<std::string>* parameters) {
if(admission == "LCD")
    return new LCD();
else if(admission == "LCE")
    return new LCE();
else if(admission == "NHIT")
    return new NHIT(parameters);
else if(admission == "PROB")
    return new PROB(parameters);
else if (admission == "RAND")
    return new RandomAdmission();
else if (admission == "MCD")
    return new MCD();
else throw "This type of admission Strategy does not exist \n";
}
