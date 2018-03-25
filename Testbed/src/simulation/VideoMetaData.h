#ifndef VIDEOMETADATA_H_
#define VIDEOMETADATA_H_
#include <string>


struct VideoMetaData {
    std::string videoId;
    std::string duration;
    std::string uploaded;
    std::string uploaded_datetime;
    std::string uploader;
    std::string category;
    int requested;

    VideoMetaData(std::string videoId, std::string duration, std::string uploaded,
            std::string uploaded_daytime, std::string uploader, std::string category, int requested) {

       this->videoId = videoId;
       this->duration = duration;
       this->uploaded = uploaded;
       this->uploaded_datetime = uploaded_daytime;
       this->uploader = uploader;
       this->category = category;
       this->requested = requested;
    }
};

#endif /* VIDEOMETADATA_H_ */
