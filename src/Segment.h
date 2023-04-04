
#ifndef RAILNETWORK_SEGMENT_H
#define RAILNETWORK_SEGMENT_H

#include <string>
#include <utility>


enum SegmentType {
    INVALID,
    STANDARD,
    ALFA_PENDULAR
};


struct Segment {
    unsigned int capacity;
    SegmentType service;

    Segment(unsigned int capacity, SegmentType service) :
            capacity(capacity),
            service(service) {};
};


#endif //RAILNETWORK_SEGMENT_H
