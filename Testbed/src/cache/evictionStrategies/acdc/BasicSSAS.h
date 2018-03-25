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

#ifndef SRC_CACHE_EVICTIONSTRATEGIES_ACDC_BASICSSAS_H_
#define SRC_CACHE_EVICTIONSTRATEGIES_ACDC_BASICSSAS_H_

class BasicSSAS {
public:
    BasicSSAS();
    virtual ~BasicSSAS();
    virtual int getIdToReduce(unsigned int id) = 0;
};

#endif /* SRC_CACHE_EVICTIONSTRATEGIES_ACDC_BASICSSAS_H_ */
