
#ifndef RAILNETWORK_RAILNETWORK_H
#define RAILNETWORK_RAILNETWORK_H

#include <list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <queue>
#include "Segment.h"


class RailNetwork { // Directed Graph
    struct Node {
        std::string name;
        std::string prev;
        std::list<std::string> adj;
        bool visited;

        Node(std::string name, std::string prev, std::list<std::string> adj, bool visited) :
            name(std::move(name)),
            prev(std::move(prev)),
            adj(std::move(adj)),
            visited(visited) {};
    };

    std::unordered_map<std::string, Node> nodes;

    const Node& getNode(const std::string& station);
    void visit(const std::string& station);
    void clearVisits();
    std::list<std::string> getAdj(const std::string& station);
public:
    void addNode(const std::string& name, const std::list<std::string>& adj);
    std::list<std::string> BFS(const std::string& src, const std::string& dest);
    int maxFlow(const std::string& origin, const std::string& destination);
    std::list<Segment> importantEdges();
    std::list<std::string> topMunicipalities(int k);
    int maxFlowStation(const std::string& station);
    int maxFlowMinCost(const std::string& origin, const std::string& destination);
    int maxFlowReduced(const std::string& origin, const std::string& destination);
    std::list<std::string> topAffectedStations(int k);

};


#endif //RAILNETWORK_RAILNETWORK_H
