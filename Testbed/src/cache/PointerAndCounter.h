/*
 * PointerAndCounter.h
 *
 *  Created on: Apr 17, 2020
 *      Author: johannes
 */

#ifndef SRC_CACHE_POINTERANDCOUNTER_H_
#define SRC_CACHE_POINTERANDCOUNTER_H_

class PointerAndCounter {
public:
    PointerAndCounter(VideoSegment* seg, int count);
    virtual ~PointerAndCounter();
    int getCount();
    void increaseCount();
    VideoSegment* getSegment();
private:
    int count;
    VideoSegment* seg;
};


#endif /* SRC_CACHE_POINTERANDCOUNTER_H_ */
