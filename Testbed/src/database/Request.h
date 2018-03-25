#include <string>

#ifndef SRC_CACHE_REQUEST_H_
#define SRC_CACHE_REQUEST_H_

struct Request{
    std::string videoId;
    int segmentId;
    long long viewedOn;
};

#endif /* SRC_CACHE_REQUEST_H_ */
