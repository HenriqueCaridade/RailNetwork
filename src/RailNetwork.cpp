
#include <stdexcept>
#include "RailNetwork.h"
#include "Segment.h"

using namespace std;


void RailNetwork::addSegment(const string& origin, const string& destination, unsigned int capacity, SegmentType service) {
    edges[origin].insert({destination, Segment(origin, destination, capacity, service)});
}

const Segment* RailNetwork::getSegment(const string &origin, const string &destination) {
    try {
        return &edges.at(origin).at(destination);
    } catch (std::out_of_range& _) {
        return nullptr;
    }
}

const RailNetwork::Node *RailNetwork::getNode(const string &station) {
    try {
        return &nodes.at(station);
    } catch (std::out_of_range& _) {
        return nullptr;
    }
}

int RailNetwork::maxFlow(const string &origin, const string &destination) {
    // TODO: [2.1]
    return 0;
}

std::list<Segment> RailNetwork::importantEdges() {
    // TODO: [2.2]
    return std::list<Segment>();
}

std::list<std::string> RailNetwork::topMunicipalities(int k) {
    // TODO: [2.3]
    return std::list<std::string>();
}

int RailNetwork::maxFlowStation(const string &station) {
    // TODO: [2.4]
    return 0;
}

int RailNetwork::maxFlowMinCost(const string &origin, const string &destination) {
    // TODO: [3.1]
    return 0;
}

int RailNetwork::maxFlowReduced(const string &origin, const string &destination) {
    // TODO: [4.1]
    return 0;
}

std::list<std::string> RailNetwork::topAffectedStations(int k) {
    // TODO: [4.2]
    return std::list<std::string>();
}
