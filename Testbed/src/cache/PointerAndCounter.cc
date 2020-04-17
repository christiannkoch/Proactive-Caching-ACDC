/*
 * PointerAndCounter.cc
 *
 *  Created on: Apr 17, 2020
 *      Author: johannes
 */

#include "PointerAndCounter.h"

PointerAndCounter::PointerAndCounter(VideoSegment* seg, int count) {
    this->count = count;
    this->seg = seg;
}

PointerAndCounter::~PointerAndCounter() {
    delete seg;
}

int PointerAndCounter::getCount(){
    return count;
}

void PointerAndCounter::increaseCount(){
    count++;
}

VideoSegment* PointerAndCounter::getSegment(){
    return this->seg;
}


