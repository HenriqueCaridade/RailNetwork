
#ifndef RAILNETWORK_RAILNETWORK_H
#define RAILNETWORK_RAILNETWORK_H

#include <list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <queue>

#include "Segment.h"
#include "Station.h"

class RailNetwork { // Directed Graph
    struct Edge {
        std::string origin;
        std::string dest;
        SegmentType type;
        const unsigned capacity;
        unsigned flow;

        Edge(std::string origin, std::string dest, SegmentType type, unsigned capacity) :
            origin(std::move(origin)),
            dest(std::move(dest)),
            type(type),
            capacity(capacity),
            flow(0) {}
    };

    struct Node {
        std::string name;
        std::string prev;
        std::list<Edge> adj;
        bool visited;

        Node(std::string name, std::string prev, std::list<Edge> adj, bool visited) :
            name(std::move(name)),
            prev(std::move(prev)),
            adj(std::move(adj)),
            visited(visited) {}
    };

    std::unordered_map<std::string, Node> nodes;

    const Node& getNode(const std::string& station);
    Edge& getEdge(const std::string& src, const std::string& dest);
    void visit(const std::string& station);
    void clearVisits();
    void clearFlow();
    std::list<Edge> getAdj(const std::string& station);
    void addEdge(const std::string& node, const Edge& edge);
public:
    void addNode(const std::string& name, const std::list<Edge>& adj);
    std::list<std::string> BFSFlow(const std::string& src, const std::string& dest);
    unsigned maxFlow(const std::string& origin, const std::string& destination);
    std::list<std::pair<std::string, std::string>> importantStations();
    std::list<std::string> topMunicipalities(int k, const std::unordered_map<std::string, Station>& stations);
    std::list<std::string> topDistricts(int k, const std::unordered_map<std::string, Station>& stations);
    int maxFlowStation(const std::string& station);
    int maxFlowMinCost(const std::string& origin, const std::string& destination);
    int maxFlowReduced(const std::string& origin, const std::string& destination);
    std::list<std::string> topAffectedStations(int k);

    friend class RailManager;
};


#endif //RAILNETWORK_RAILNETWORK_H
