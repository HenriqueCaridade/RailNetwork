
#ifndef RAILNETWORK_RAILNETWORK_H
#define RAILNETWORK_RAILNETWORK_H

#include <list>
#include <string>
#include <unordered_map>

#include "Segment.h"


class RailNetwork { // Directed Graph
    struct Node {
        std::list<std::string> adj;
        bool visited;
    };

    std::unordered_map<std::string, Node> nodes;
    std::unordered_map<std::string, std::unordered_map<std::string, Segment>> edges;

    const Node* getNode(const std::string& station);
    const Segment* getSegment(const std::string& origin, const std::string& destination);
public:
    void addSegment(const std::string& origin, const std::string& destination, unsigned int capacity, SegmentType service);
    int maxFlow(const std::string& origin, const std::string& destination);
    std::list<Segment> importantEdges();
    std::list<std::string> topMunicipalities(int k);
    int maxFlowStation(const std::string& station);
    int maxFlowMinCost(const std::string& origin, const std::string& destination);
    int maxFlowReduced(const std::string& origin, const std::string& destination);
    std::list<std::string> topAffectedStations(int k);

};


#endif //RAILNETWORK_RAILNETWORK_H
