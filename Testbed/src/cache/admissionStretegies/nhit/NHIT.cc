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
 * @author Johannes Pfannmüller
 * @date
 * @version 1.0
 *
 * @brief Implements Leave Copy Down Strategy
 *
 * @section DESCRIPTION
 *
 * This is a Class responsible for implementing the behaviour of the NHIT Strategy.
 * Its main responsibility is to return a boolean value indicating if the video segment
 * has passed this reverse proxy a certain number of times in a given timeframe.
 *
 */

#include "NHIT.h"
#include <vector>
#include <string>
#include <omnetpp.h>

NHIT::NHIT(std::vector<std::string>* parameters) {
    // TODO Auto-generated constructor stub
    this->thresholdValue = std::stoi(parameters->at(0));
    this->timeFrame = std::stod(parameters->at(1));//259200.00;
}

NHIT::~NHIT() {
    // TODO Auto-generated destructor stub
    for (auto p : seenPackets) {
        delete p.second;
        //seenPackets.erase(p.first);
    }

}
/**
 * @brief decides if the copy of a given video segment will be stored
 *
 * A segment is cached if it is seen thresholdValue+1 times in the timeFrame.
 * @param pkg is a pointer to an initialized VideoSegment
 * @return boolean signaling if a copy of the video segment should be stored
 */
bool NHIT::toBeCached(VideoSegment* pkg) {
    std::string keyBuilder = pkg->getVideoId()
            + std::to_string(pkg->getSegmentId());
    if (seenPackets.find(keyBuilder) == seenPackets.end()) {
        std::vector<double>* newVec = new std::vector<double>();
        newVec->push_back(omnetpp::simTime().dbl());
        seenPackets[keyBuilder] = newVec;
        return false;
    } else {
        seenPackets[keyBuilder]->push_back(omnetpp::simTime().dbl());
        for (unsigned int i = 0; i < seenPackets[keyBuilder]->size()-1; i++) {
            if (seenPackets[keyBuilder]->at(i)
                    < omnetpp::simTime().dbl() - timeFrame) {
                seenPackets[keyBuilder]->erase(
                        seenPackets[keyBuilder]->begin() + i);
            }
        }

        if (seenPackets[keyBuilder]->size() > thresholdValue) {
            delete seenPackets[keyBuilder];
            seenPackets.erase(keyBuilder);
            return true;
        }
        return false;
    }
}


/**
 * @brief periodic events
 * With NHIT the periodic events are responsible to purge the storage of expired videoIDs
 *
 * @return void
 */
void NHIT::periodicEvents() {
    for (auto j : seenPackets) {
        for (unsigned int i = 0; i < j.second->size(); i++) {
            if (j.second->at(i) < omnetpp::simTime().dbl() - timeFrame) {
                j.second->erase(j.second->begin() + i);
            }
        }
        if (j.second->size() == 0) {
            std::string toDelete = j.first;
            delete j.second;
            stringsToDelete.push_back(toDelete);
            //seenPackets.erase(toDelete);
        }
    }
    for (auto i : stringsToDelete){
        seenPackets.erase(i);
    }
    stringsToDelete.clear();
}
