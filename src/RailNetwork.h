
#ifndef RAILNETWORK_RAILNETWORK_H
#define RAILNETWORK_RAILNETWORK_H

#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include "Segment.h"


class RailNetwork { // Directed Graph
    struct Node {
        std::list<std::string> adj;
        bool visited;
        std::string prev;
    };

    std::unordered_map<std::string, Node> nodes;
    std::unordered_map<std::string, std::unordered_map<std::string, Segment>> edges; // 1st key- origin 2st key- destination

    const Node* getNode(const std::string& station);
    const Segment* getSegment(const std::string& origin, const std::string& destination);
    void visit(const std::string& station);
    void clearVisits();
    std::list<std::string> getAdj(const std::string& station);
public:
    std::list<std::string> BFS(const std::string& src, const std::string& dest);
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
