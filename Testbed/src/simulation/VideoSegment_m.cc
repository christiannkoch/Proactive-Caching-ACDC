//
// Generated file, do not edit! Created by nedtool 5.2 from src/simulation/VideoSegment.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "VideoSegment_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(VideoSegment)

VideoSegment::VideoSegment(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    this->count = 1;
    this->clientId = 1;
    this->segmentId = 1;
    this->videoId = "";
    this->duration = "";
    this->uploaded = "";
    this->uploaded_datetime = "";
    this->uploader = "";
    this->category = "";
    this->requested = 1;
    this->seenAbove = false;
    this->size = 75;
}

VideoSegment::VideoSegment(const VideoSegment& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

VideoSegment::~VideoSegment()
{
}

VideoSegment& VideoSegment::operator=(const VideoSegment& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void VideoSegment::copy(const VideoSegment& other)
{
    this->count = other.count;
    this->clientId = other.clientId;
    this->segmentId = other.segmentId;
    this->videoId = other.videoId;
    this->duration = other.duration;
    this->uploaded = other.uploaded;
    this->uploaded_datetime = other.uploaded_datetime;
    this->uploader = other.uploader;
    this->category = other.category;
    this->requested = other.requested;
    this->seenAbove = other.seenAbove;
    this->size = other.size;
}

void VideoSegment::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->count);
    doParsimPacking(b,this->clientId);
    doParsimPacking(b,this->segmentId);
    doParsimPacking(b,this->videoId);
    doParsimPacking(b,this->duration);
    doParsimPacking(b,this->uploaded);
    doParsimPacking(b,this->uploaded_datetime);
    doParsimPacking(b,this->uploader);
    doParsimPacking(b,this->category);
    doParsimPacking(b,this->requested);
    doParsimPacking(b,this->seenAbove);
    doParsimPacking(b,this->size);
}

void VideoSegment::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->count);
    doParsimUnpacking(b,this->clientId);
    doParsimUnpacking(b,this->segmentId);
    doParsimUnpacking(b,this->videoId);
    doParsimUnpacking(b,this->duration);
    doParsimUnpacking(b,this->uploaded);
    doParsimUnpacking(b,this->uploaded_datetime);
    doParsimUnpacking(b,this->uploader);
    doParsimUnpacking(b,this->category);
    doParsimUnpacking(b,this->requested);
    doParsimUnpacking(b,this->seenAbove);
    doParsimUnpacking(b,this->size);
}

int VideoSegment::getCount() const
{
    return this->count;
}

void VideoSegment::setCount(int count)
{
    this->count = count;
}

int VideoSegment::getClientId() const
{
    return this->clientId;
}

void VideoSegment::setClientId(int clientId)
{
    this->clientId = clientId;
}

int VideoSegment::getSegmentId() const
{
    return this->segmentId;
}

void VideoSegment::setSegmentId(int segmentId)
{
    this->segmentId = segmentId;
}

const char * VideoSegment::getVideoId() const
{
    return this->videoId.c_str();
}

void VideoSegment::setVideoId(const char * videoId)
{
    this->videoId = videoId;
}

const char * VideoSegment::getDuration() const
{
    return this->duration.c_str();
}

void VideoSegment::setDuration(const char * duration)
{
    this->duration = duration;
}

const char * VideoSegment::getUploaded() const
{
    return this->uploaded.c_str();
}

void VideoSegment::setUploaded(const char * uploaded)
{
    this->uploaded = uploaded;
}

const char * VideoSegment::getUploaded_datetime() const
{
    return this->uploaded_datetime.c_str();
}

void VideoSegment::setUploaded_datetime(const char * uploaded_datetime)
{
    this->uploaded_datetime = uploaded_datetime;
}

const char * VideoSegment::getUploader() const
{
    return this->uploader.c_str();
}

void VideoSegment::setUploader(const char * uploader)
{
    this->uploader = uploader;
}

const char * VideoSegment::getCategory() const
{
    return this->category.c_str();
}

void VideoSegment::setCategory(const char * category)
{
    this->category = category;
}

int VideoSegment::getRequested() const
{
    return this->requested;
}

void VideoSegment::setRequested(int requested)
{
    this->requested = requested;
}

bool VideoSegment::getSeenAbove() const
{
    return this->seenAbove;
}

void VideoSegment::setSeenAbove(bool seenAbove)
{
    this->seenAbove = seenAbove;
}

int VideoSegment::getSize() const
{
    return this->size;
}

void VideoSegment::setSize(int size)
{
    this->size = size;
}

class VideoSegmentDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    VideoSegmentDescriptor();
    virtual ~VideoSegmentDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(VideoSegmentDescriptor)

VideoSegmentDescriptor::VideoSegmentDescriptor() : omnetpp::cClassDescriptor("VideoSegment", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

VideoSegmentDescriptor::~VideoSegmentDescriptor()
{
    delete[] propertynames;
}

bool VideoSegmentDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<VideoSegment *>(obj)!=nullptr;
}

const char **VideoSegmentDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *VideoSegmentDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int VideoSegmentDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 12+basedesc->getFieldCount() : 12;
}

unsigned int VideoSegmentDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<12) ? fieldTypeFlags[field] : 0;
}

const char *VideoSegmentDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "count",
        "clientId",
        "segmentId",
        "videoId",
        "duration",
        "uploaded",
        "uploaded_datetime",
        "uploader",
        "category",
        "requested",
        "seenAbove",
        "size",
    };
    return (field>=0 && field<12) ? fieldNames[field] : nullptr;
}

int VideoSegmentDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "count")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "clientId")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "segmentId")==0) return base+2;
    if (fieldName[0]=='v' && strcmp(fieldName, "videoId")==0) return base+3;
    if (fieldName[0]=='d' && strcmp(fieldName, "duration")==0) return base+4;
    if (fieldName[0]=='u' && strcmp(fieldName, "uploaded")==0) return base+5;
    if (fieldName[0]=='u' && strcmp(fieldName, "uploaded_datetime")==0) return base+6;
    if (fieldName[0]=='u' && strcmp(fieldName, "uploader")==0) return base+7;
    if (fieldName[0]=='c' && strcmp(fieldName, "category")==0) return base+8;
    if (fieldName[0]=='r' && strcmp(fieldName, "requested")==0) return base+9;
    if (fieldName[0]=='s' && strcmp(fieldName, "seenAbove")==0) return base+10;
    if (fieldName[0]=='s' && strcmp(fieldName, "size")==0) return base+11;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *VideoSegmentDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "string",
        "string",
        "string",
        "string",
        "string",
        "string",
        "int",
        "bool",
        "int",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : nullptr;
}

const char **VideoSegmentDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *VideoSegmentDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int VideoSegmentDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    VideoSegment *pp = (VideoSegment *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *VideoSegmentDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    VideoSegment *pp = (VideoSegment *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string VideoSegmentDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    VideoSegment *pp = (VideoSegment *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getCount());
        case 1: return long2string(pp->getClientId());
        case 2: return long2string(pp->getSegmentId());
        case 3: return oppstring2string(pp->getVideoId());
        case 4: return oppstring2string(pp->getDuration());
        case 5: return oppstring2string(pp->getUploaded());
        case 6: return oppstring2string(pp->getUploaded_datetime());
        case 7: return oppstring2string(pp->getUploader());
        case 8: return oppstring2string(pp->getCategory());
        case 9: return long2string(pp->getRequested());
        case 10: return bool2string(pp->getSeenAbove());
        case 11: return long2string(pp->getSize());
        default: return "";
    }
}

bool VideoSegmentDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    VideoSegment *pp = (VideoSegment *)object; (void)pp;
    switch (field) {
        case 0: pp->setCount(string2long(value)); return true;
        case 1: pp->setClientId(string2long(value)); return true;
        case 2: pp->setSegmentId(string2long(value)); return true;
        case 3: pp->setVideoId((value)); return true;
        case 4: pp->setDuration((value)); return true;
        case 5: pp->setUploaded((value)); return true;
        case 6: pp->setUploaded_datetime((value)); return true;
        case 7: pp->setUploader((value)); return true;
        case 8: pp->setCategory((value)); return true;
        case 9: pp->setRequested(string2long(value)); return true;
        case 10: pp->setSeenAbove(string2bool(value)); return true;
        case 11: pp->setSize(string2long(value)); return true;
        default: return false;
    }
}

const char *VideoSegmentDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *VideoSegmentDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    VideoSegment *pp = (VideoSegment *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


