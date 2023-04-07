
#ifndef RAILNETWORK_SEGMENT_H
#define RAILNETWORK_SEGMENT_H

#include <string>
#include <utility>


enum SegmentType {
    INVALID,
    STANDARD,
    ALFA_PENDULAR
};

/**
*@brief Represents a segment of a railway network connecting two stations.
*/
struct Segment {
    std::string origin;
    std::string destination;
    unsigned int capacity;
    SegmentType service;
/**
*@brief Constructs a new Segment object.
*@param origin The name of the origin station.
*@param destination The name of the destination station.
*@param capacity The maximum capacity of the segment.
*@param service The type of service provided by the segment.
*/
    Segment(std::string origin, std::string destination, unsigned int capacity, SegmentType service) :
            origin(std::move(origin)),
            destination(std::move(destination)),
            capacity(capacity),
            service(service) {};

};


#endif //RAILNETWORK_SEGMENT_H
