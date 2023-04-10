
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
/**
 * RailNetwork class represents a directed graph that models a rail network system. It provides methods to calculate
 * maximum flow and other network analysis functions.
 */
class RailNetwork { // Directed Graph
    static const std::string sourceNodeName;
    /**
     * @brief A struct to represent an edge in the graph.
     */
    struct Edge {
        std::string origin;
        std::string dest;
        SegmentType type;
        const unsigned capacity;
        unsigned flow;
        bool active;
        /**
         * @brief Constructs an Edge object with the given parameters.
         * @param origin The name of the origin node of the edge.
         * @param dest The name of the destination node of the edge.
         * @param type The type of the segment of the edge.
         * @param capacity The capacity of the edge.
         */
        Edge(std::string origin, std::string dest, SegmentType type, unsigned capacity) :
            origin(std::move(origin)),
            dest(std::move(dest)),
            type(type),
            capacity(capacity),
            active(true),
            flow(0) {}
    };
    /**
     * @brief A struct to represent a node in the graph.
     */
    struct Node {
        std::string name;
        std::list<Edge> adj;
        std::string prev;
        std::string prevStandard;
        std::string prevAlfa;
        bool visited;
        bool visitedStandard;
        bool visitedAlfa;
        unsigned cost;
        bool active;
        /**
         * @brief Constructs a Node object with the given parameters.
         * @param name The name of the node.
         * @param adj A list of edges that are adjacent to the node.
         */
        Node(std::string name, std::list<Edge> adj) :
            name(std::move(name)),
            adj(std::move(adj)),
            prev(""),
            prevStandard(""),
            prevAlfa(""),
            visited(false),
            visitedStandard(false),
            visitedAlfa(false),
            active(true),
            cost(UINT_MAX){}
    };

    std::unordered_map<std::string, Node> nodes;
    /**
     * @brief Gets the node with the given name from the nodes map.
     * @param station The name of the node.
     * @return A reference to the node.
     */
    Node& getNode(const std::string& station);
    /**
     * @brief Gets the edge between the two given nodes.
     * @param src The name of the source node of the edge.
     * @param dest The name of the destination node of the edge.
     * @return A reference to the edge.
     */
    Edge& getEdge(const std::string& src, const std::string& dest);
    /**
     * @brief Marks the node with the given name as visited.
     * @param station The name of the node to mark as visited.
     * @param type The type of train.
     */
    void visit(const std::string& station, SegmentType type = INVALID);
    /**
     * @brief Returns if given node is visited.
     * @param station The name of the node to mark as visited.
     * @param type The type of train.
     * @return is Node visited?
     */
    bool isVisited(const std::string &station, SegmentType type = INVALID);
    /**
     * @brief Marks all nodes as not visited.
     */
    void clearVisits();
    /**
     * @brief Clears prev variables.
     */
    void clearPrevs();
    /**
     * @brief Clears the flow of all edges in the graph.
     */
    void clearFlow();
    /**
     * @brief Clears the cost values for all nodes in the graph.
     */
    void clearCost();
    /**
     * @brief Sets the cost of the node with the given name.
     * @param node The name of the node to set the cost for.
     * @param cost The cost to set for the node.
     */
    void setCost(const std::string& node, unsigned cost);
    /**
     * @brief Sets the previous node for the node with the given name.
     * @param node The name of the node to set the previous node for.
     * @param prev The name of the previous node.
     */
    void setPrev(const std::string& node, const std::string& prev, SegmentType type);
    /**
     * @brief Returns the cost of the node with the given name.
     * @param node The name of the node to return the cost for.
     * @return The cost of the node.
     */
    unsigned getCost(const std::string& node);
    /**
     * @brief Returns a list of edges adjacent to the node with the given name.
     * @param station The name of the node to return the adjacent edges for.
     * @return A list of adjacent edges.
     */
    std::list<Edge> getAdj(const std::string& station);
    /**
     * @brief Adds an edge to the graph.
     * @param node The name of the node to add the edge to.
     * @param edge The edge to add.
     */
    void addEdge(const std::string& node, const Edge& edge);
    /**
     * @brief Uses Breadth-First Search to find the path with maximum flow from the given source to destination node.
     * @param src The name of the source node.
     * @param dest The name of the destination node.
     * @return A list of node names representing the path with maximum flow.
     */
    std::list<std::string> BFSFlow(const std::string& src, const std::string& dest);
    /**
     * @brief Uses Breadth-First Search to find all paths with minimum cost from the given source to destination node.
     * @param src The name of the source node.
     * @param dest The name of the destination node.
     * @return A list of lists of node names representing all paths with minimum cost.
     */
    std::list<std::list<std::string>> BFSCost(const std::string &src, const std::string &dest);
    /**
     * @brief Uses Breadth-First Search to find the shortest path from the given source to destination node, considering only active edges.
     * @param src The name of the source node.
     * @param dest The name of the destination node.
     * @return A list of node names representing the shortest path.
     */
    std::list<std::string> BFSActive(const std::string &src, const std::string &dest);
    /**
     * Returns a list of all nodes that are at a specified distance from the source node.
     * @param src The name of the source node.
     * @param distance The specified distance from the source node.
     * @return A list of all nodes at the specified distance from the source node.
     */
    std::list<std::string> distancedNodes(const std::string& src, unsigned distance);
public:
    /**
     * Adds a node with the specified name and list of adjacent edges to the rail network.
     * @param name The name of the node to be added.
     * @param adj The list of adjacent edges for the new node.
     */
    void addNode(const std::string& name, const std::list<Edge>& adj);
    /**
     * Calculates and returns the maximum flow between two nodes in the rail network using the Ford-Fulkerson algorithm.
     * @param origin The name of the origin node.
     * @param destination The name of the destination node.
     * @return The maximum flow between the origin and destination nodes.
     */
    unsigned maxFlow(const std::string& origin, const std::string& destination);
    /**
     * Returns a list of all important stations in the rail network. Importance is based on the number of paths that pass through the station.
     * @return A pair of the list of all important stations in the rail network and the maxFlow between them.
     */
    std::pair<std::list<std::pair<std::string, std::string>>, unsigned> importantStations();
    /**
     * Returns a list of the top k municipalities in the rail network based on the number of stations within their borders.
     * @param k The number of top municipalities to return.
     * @param stations An unordered map of station names to station objects.
     * @return A list of the top k municipalities in the rail network.
     */
    std::list<std::pair<std::string, unsigned>> topMunicipalities(int k, const std::unordered_map<std::string, Station>& stations);
    /**
     * Returns a list of the top k districts in the rail network based on the number of stations within their borders.
     * @param k The number of top districts to return.
     * @param stations An unordered map of station names to station objects.
     * @return A list of the top k districts in the rail network.
     */
    std::list<std::pair<std::string, unsigned>> topDistricts(int k, const std::unordered_map<std::string, Station>& stations);
    /**
     * Calculates and returns the maximum flow that passes through a specific station in the rail network.
     * @param station The name of the station.
     * @return The maximum flow that passes through the station.
     */
    unsigned maxFlowStation(const std::string& station);
    /**
     * Calculates and returns the maximum flow between two nodes in the rail network using the Ford-Fulkerson algorithm with minimum cost.
     * @param origin The name of the origin node.
     * @param destination The name of the destination node.
     * @return The maximum flow between the origin and destination nodes with minimum cost.
     */
    unsigned maxFlowMinCost(const std::string& origin, const std::string& destination);
    /**
     * Calculates and returns the maximum flow between two nodes in the rail network using the reduced-cost augmenting path algorithm.
     * @param origin The name of the origin node.
     * @param destination The name of the destination node.
     * @return The maximum flow between the origin and destination nodes.
     */
    unsigned maxFlowReduced(const std::string& origin, const std::string& destination);
    /**
     * Returns a list of the top k affected stations, i.e. stations with the highest total flow of passengers
     * in both directions during the day.
     * @param k The number of stations to return.
     * @param stations The map of stations.
     * @return A list of the names of the top k affected stations.
     */
    std::list<std::pair<std::string, unsigned>> topAffectedStations(int k,  const std::unordered_map<std::string, Station>& stations );

    friend class RailManager;
    friend class App;
};


#endif //RAILNETWORK_RAILNETWORK_H
