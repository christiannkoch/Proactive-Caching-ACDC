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

#include "ProbationaryCacheFactory.h"
#include <string>
#include "SLRUProbationaryCache.h"
#include "ARCProbationaryCache.h"

ProbationaryCacheFactory::ProbationaryCacheFactory() {
    // TODO Auto-generated constructor stub

}
/**
 * @brief returns a specific implementation of a probationary cache
 *
 * Different eviction strategies may need different implementations of a probationary cache. this method creates the specific one
 *
 * @param size the size of the probationary cache
 * @param a string representing the type of probationary cache
 *
 * @return a pointer to a BasicProbationaryCache
 */
BasicProbationaryCache* ProbationaryCacheFactory::createProbationaryCache(long long size, std::string type){
    if(type == "SLRU")
        return new SLRUProbationaryCache(size);
    else if(type == "ARC")
        return new ARCProbationaryCache(size);
    else
            throw "This type of ProbationaryCache does not exist\n";
}

