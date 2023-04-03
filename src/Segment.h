
#ifndef RAILNETWORK_SEGMENT_H
#define RAILNETWORK_SEGMENT_H

#include <string>
#include <utility>


enum SegmentType {
    STANDARD,
    ALFA_PENDULAR,
    INVALID
};


struct Segment {
    std::string origin;
    std::string destination;
    unsigned int capacity;
    SegmentType service;

    Segment(std::string origin, std::string destination, unsigned int capacity, SegmentType service) :
            origin(std::move(origin)),
            destination(std::move(destination)),
            capacity(capacity),
            service(service) {};
};


#endif //RAILNETWORK_SEGMENT_H
