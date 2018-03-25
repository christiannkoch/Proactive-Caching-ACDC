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

#include "ProbationaryFirstSSAS.h"

ProbationaryFirstSSAS::ProbationaryFirstSSAS(ARCProbationaryCache* probationaryCache,
        BasicGhostList* probationaryGhostList,
        std::vector<BasicCacheSegment*>* cacheSegmentVector,
        std::vector<BasicGhostList*>* ghostListVector, unsigned int cacheSize,
        double subCacheSize, int minSegSize) {
    // TODO Auto-generated constructor stub

}

ProbationaryFirstSSAS::~ProbationaryFirstSSAS() {
    // TODO Auto-generated destructor stub
}

int ProbationaryFirstSSAS::getIdToReduce(unsigned int id){
    return 0;
}
