
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
    static const std::string sourceNodeName;
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
        unsigned cost;

        Node(std::string name, std::list<Edge> adj) :
            name(std::move(name)),
            adj(std::move(adj)),
            prev(""),
            visited(false),
            cost(UINT_MAX){}
    };

    std::unordered_map<std::string, Node> nodes;

    const Node& getNode(const std::string& station);
    Edge& getEdge(const std::string& src, const std::string& dest);
    void visit(const std::string& station);
    void clearVisits();
    void clearFlow();
    void clearCost();
    void setCost(const std::string& node, unsigned cost);
    void setPrev(const std::string& node, const std::string& prev);
    unsigned getCost(const std::string& node);
    std::list<Edge> getAdj(const std::string& station);
    void addEdge(const std::string& node, const Edge& edge);
    std::list<std::string> BFSFlow(const std::string& src, const std::string& dest);
    std::list<std::list<std::string>> BFSCost(const std::string &src, const std::string &dest);
    std::list<std::string> distancedNodes(const std::string& src, unsigned distance);
public:
    void addNode(const std::string& name, const std::list<Edge>& adj);
    unsigned maxFlow(const std::string& origin, const std::string& destination);
    std::list<std::pair<std::string, std::string>> importantStations();
    std::list<std::string> topMunicipalities(int k, const std::unordered_map<std::string, Station>& stations);
    std::list<std::string> topDistricts(int k, const std::unordered_map<std::string, Station>& stations);
    unsigned maxFlowStation(const std::string& station);
    unsigned maxFlowMinCost(const std::string& origin, const std::string& destination);
    unsigned maxFlowReduced(const std::string& origin, const std::string& destination);
    std::list<std::string> topAffectedStations(int k);

    friend class RailManager;
};


#endif //RAILNETWORK_RAILNETWORK_H
