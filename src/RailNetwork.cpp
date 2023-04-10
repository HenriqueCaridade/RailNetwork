
#include <stdexcept>
#include <unordered_set>
#include <set>
#include <queue>
#include <algorithm>
#include <iostream>

#include "RailNetwork.h"
#include "Segment.h"

using namespace std;


// Compare Classes for Priority Queues //
template<class T>
class GreaterCompare {
public:
    bool operator() (const pair<T,unsigned>& p1, const pair<T, unsigned>& p2) const {
        return p1.second > p2.second;
    }
};

template<class T>
class LessCompare {
public:
    bool operator() (const pair<T,unsigned>& p1, const pair<T, unsigned>& p2) const {
        return p1.second < p2.second;
    }
};


const std::string RailNetwork::sourceNodeName = "__SOURCE_NODE_NAME__";

RailNetwork::Node& RailNetwork::getNode(const string &station) {
    return nodes.at(station);
}

void RailNetwork::addNode(const std::string& name, const std::list<Edge>& adj) {
    nodes.insert({name, Node(name, adj)});
}

RailNetwork::Edge& RailNetwork::getEdge(const std::string& src, const string &dest) {
    for (Edge& edge : getNode(src).adj)
        if (edge.dest == dest)
            return edge;
    throw std::out_of_range("Didn't Find the Edge.");
}

void RailNetwork::visit(const std::string& station, SegmentType type){
    switch (type) {
        case INVALID: getNode(station).visited = true; break;
        case STANDARD: getNode(station).visitedStandard = true; break;
        case ALFA_PENDULAR: getNode(station).visitedAlfa = true; break;
    }
}

bool RailNetwork::isVisited(const std::string& station, SegmentType type){
    switch (type) {
        case INVALID: return nodes.at(station).visited;
        case STANDARD: return nodes.at(station).visitedStandard;
        case ALFA_PENDULAR: return nodes.at(station).visitedAlfa;
    }
}

void RailNetwork::clearVisits() {
    for(auto& p : nodes) {
        p.second.visited = false;
        p.second.visitedStandard = false;
        p.second.visitedAlfa = false;
    }
}

void RailNetwork::clearPrevs() {
    for(auto& p : nodes) {
        p.second.prev = "";
        p.second.prevStandard = "";
        p.second.prevAlfa = "";
    }
}

void RailNetwork::clearFlow() {
    for(auto& [_,node] : nodes)
        for(Edge& e : node.adj)
            e.flow = 0;
}

void RailNetwork::clearCost() {
    for(auto& [_,node] : nodes)
        node.cost = UINT_MAX;
}

void RailNetwork::setCost(const string& node, unsigned cost) {
    nodes.at(node).cost = cost;
}

unsigned RailNetwork::getCost(const string& node) {
    return nodes.at(node).cost;
}

void RailNetwork::setPrev(const string &node, const string &prev, SegmentType type) {
    switch (type) {
        case INVALID: getNode(node).prev = prev; break;
        case STANDARD: getNode(node).prevStandard = prev; break;
        case ALFA_PENDULAR: getNode(node).prevAlfa = prev; break;
    }
}

list<RailNetwork::Edge> RailNetwork::getAdj(const string &station) {
    return getNode(station).adj;
}

void RailNetwork::addEdge(const string &node, const Edge &edge) {
    getNode(node).adj.push_back(edge);
}

// []===========================================[] //
// ||                    BFSs                   || //
// []===========================================[] //


list<string> RailNetwork::BFSFlow(const string &src, const string &dest) {
    clearVisits();
    clearPrevs();
    queue<pair<string, SegmentType>> q;
    q.push({src, INVALID});
    list<string> res;
    visit(src);
    while (!q.empty()) { // No more Nodes
        auto [curr, type] = q.front();
        visit(curr, type);
        list<Edge> edges = getAdj(curr);
        q.pop();
        for(const Edge& edge : edges) {
            if (type != INVALID && (type != edge.type)) continue; // Different Train
            if (edge.flow == edge.capacity) continue; // if segment flow is full dont add node to queue
            if (isVisited(edge.dest)) continue;
            if (isVisited(edge.dest, edge.type)) continue;
            setPrev(edge.dest, curr, edge.type);
            q.push({edge.dest, edge.type});
            if (edge.dest == dest) goto found_destination;
        }
    }
    goto skip_found_destination;
    found_destination:
    res.push_front(dest);
    if (getNode(dest).prevAlfa.empty()){ // Standard
        while (true) {
            if (res.front() == src) break;
            Node& aux = getNode(res.front());
            string prev = aux.prevStandard.empty() ? aux.prev : aux.prevStandard;
            res.push_front(prev);
        }
    } else { // Alfa
        while (true) {
            if (res.front() == src) break;
            Node& aux = getNode(res.front());
            string prev = aux.prevAlfa.empty() ? aux.prev : aux.prevAlfa;
            res.push_front(prev);
        }
    }
    skip_found_destination:
    return res;
}

static unsigned getCostByType(SegmentType type){
    switch (type) {
        case INVALID:
            return 0;
        case STANDARD:
            return 2;
        case ALFA_PENDULAR:
            return 4;
    }
}

list<list<string>> RailNetwork::BFSCost(const string &src, const string &dest) {
    clearCost();
    priority_queue<pair<pair<list<string>, SegmentType>, unsigned>, vector<pair<pair<list<string>, SegmentType>, unsigned>>, GreaterCompare<pair<list<string>, SegmentType>>> q;
    q.push({{{src}, INVALID}, 0});
    setCost(src, 0);
    list<list<string>> res;
    unsigned minCost = UINT_MAX;
    while (!q.empty()) { // No more Nodes
        auto [p, cost] = q.top();
        auto [curr, type] = p;
        q.pop();
        if (cost > minCost) break;
        if (curr.back() == dest) {
            res.push_back(curr);
            minCost = cost;
            continue;
        }
        list<Edge> edges = getAdj(curr.back());
        unsigned newCost = cost + getCostByType(type);
        for (const Edge& edge : edges) {
            if (!type && (type != edge.type)) continue; // Different Train
            if (newCost <= getCost(edge.dest)) { // Better Path
                setCost(edge.dest, newCost);
                curr.push_back(edge.dest);
                q.push({{curr, type}, newCost});
                curr.pop_back();
            }
        }
    }
    return res;
}

list<string> RailNetwork::BFSActive(const string &src, const string &dest) {
    clearVisits();
    queue<pair<string, SegmentType>> q;
    q.push({src, INVALID});
    list<string> res;
    while (!q.empty()) { // No more Nodes
        auto [curr, type] = q.front();
        visit(curr, type);
        list<Edge> edges = getAdj(curr);
        q.pop();
        for(const Edge& edge : edges) {
            if (!type && (type != edge.type)) continue; // Different Train
            if (!edge.active) continue; // if edge is deactivated
            if (!getNode(edge.dest).active) continue; // if destination station is deactivated
            if (edge.flow == edge.capacity) continue; // if segment flow is full dont add node to queue
            if (isVisited(edge.dest)) continue;
            if (isVisited(edge.dest, edge.type)) continue;
            setPrev(edge.dest, curr, edge.type);
            q.push({edge.dest, type});
            if (edge.dest == dest) goto found_destination;
        }
    }
    goto skip_found_destination;
    found_destination:
    res.push_front(dest);
    if (getNode(dest).prevAlfa.empty()){ // Standard
        while (true) {
            if (res.front() == src) break;
            Node& aux = getNode(res.front());
            string prev = aux.prevStandard.empty() ? aux.prev : aux.prevStandard;
            res.push_front(prev);
        }
    } else { // Alfa
        while (true) {
            if (res.front() == src) break;
            Node& aux = getNode(res.front());
            string prev = aux.prevAlfa.empty() ? aux.prev : aux.prevAlfa;
            res.push_front(prev);
        }
    }
    skip_found_destination:
    return res;
}

list<string> RailNetwork::distancedNodes(const string& src, unsigned distance) {
    clearVisits();
    queue<pair<string, unsigned>> q;
    q.push({src, 0});
    list<string> res;
    while (!q.empty()) { // No more Nodes
        auto [curr, dist] = q.front();
        visit(curr);
        q.pop();
        if (dist > distance) break; // No more distant nodes
        if (dist == distance) res.push_back(curr);
        for (const Edge& edge : getAdj(curr))
            if (!isVisited(edge.dest))
                q.push({edge.dest, dist + 1});
    }
    return res;
}

// []===========================================[] //
// ||          ALGORITHMIC FUNCTIONS            || //
// []===========================================[] //

unsigned RailNetwork::maxFlow(const string &origin, const string &destination) {
    // Exercise [2.1]
    clearFlow();
    unsigned maxFlow = 0;
    while(true){
        list<string> res = BFSFlow(origin, destination);
        if (res.empty()) break;
        unsigned bottleneck = UINT_MAX;
        auto it = res.begin();
        auto itNext = res.begin();
        itNext++;
        while (itNext != res.end()) { // find bottleneck in the shortest path
            const Edge& edge = getEdge(*it, *itNext);
            unsigned remaining = edge.capacity - edge.flow;
            if (remaining < bottleneck) bottleneck = remaining;
            it++; itNext++;
        }
        it = res.begin();
        itNext = res.begin();
        itNext++;
        maxFlow += bottleneck;
        while (itNext != res.end()) { // find bottleneck in the shortest path
            Edge& edge = getEdge(*it, *itNext);
            edge.flow += bottleneck;
            it++; itNext++;
        }
    }
    return maxFlow;
}

pair<list<pair<string,string>>, unsigned> RailNetwork::importantStations() {
    // Exercise [2.2]
    unsigned maxF = 0;
    list<pair<string, string>> pairs;
    multiset<pair<string, unsigned>, GreaterCompare<string>> orderedNodes;
    for (const auto& [name, node] : nodes) {
        unsigned aux = 0;
        for (const auto& e : node.adj) aux += e.capacity;
        orderedNodes.insert({name, aux});
    }
    for (const auto& [name1, outDeg1] : orderedNodes) {
        for (const auto& [name2, outDeg2] : orderedNodes) {
            if (name1 == name2) continue;
            if (outDeg1 < maxF) continue;
            unsigned flow = maxFlow(name1, name2);
            if (flow > maxF) {
                pairs = list<pair<string, string>>{{name1, name2}};
                maxF = flow;
            } else if (flow == maxF) pairs.emplace_back(name1, name2);
        }
    }
    return {pairs, maxF};
}

list<pair<string, unsigned>> RailNetwork::topMunicipalities(int k, const unordered_map<string, Station>& stations) {
    // Exercise [2.3]
    // Where should management assign larger budgets?
    // Ans: To municipalities where there are more trains (Max Flow).
    unordered_map<string, RailNetwork> municipalities;
    for (auto& [name, node] : nodes) {
        const string& municipality = stations.at(name).municipality;
        municipalities[municipality].addNode(name, {});
        for (const auto& edge : node.adj)
            if (stations.at(edge.dest).municipality == municipality)
                municipalities.at(municipality).addEdge(name, edge);
    }
    priority_queue<pair<string, unsigned>, vector<pair<string, unsigned>>, LessCompare<string>> munMaxFlows;
    for (auto& [municipality, graph] : municipalities) {
        unsigned maxF = 0;
        multiset<pair<string, unsigned>, GreaterCompare<string>> orderedNodes;
        for (const auto& [name, node] : graph.nodes) {
            unsigned aux = 0;
            for (const auto& e : node.adj) aux += e.capacity;
            orderedNodes.insert({name, aux});
        }
        for (const auto& [name1, outDeg1] : orderedNodes) {
            for (const auto& [name2, outDeg2] : orderedNodes) {
                if (name1 == name2) continue;
                if (outDeg1 < maxF) continue;
                unsigned flow = graph.maxFlow(name1, name2);
                if (flow > maxF) maxF = flow;
            }
        }
        munMaxFlows.push({municipality, maxF});
    }
    list<pair<string, unsigned>> res;
    for (int i = 0; i < k; i++) {
        if (munMaxFlows.empty()) break;
        res.push_back(munMaxFlows.top());
        munMaxFlows.pop();
    }
    return res;
}
list<pair<string, unsigned>> RailNetwork::topDistricts(int k, const unordered_map<string, Station>& stations) {
    // Exercise [2.3]
    // Where should management assign larger budgets?
    // Ans: To districts where there are more trains (Max Flow).
    unordered_map<string, RailNetwork> districts;
    for (auto& [name, node] : nodes) {
        const string& district = stations.at(name).district;
        districts[district].addNode(name, {});
        for (const auto& edge : node.adj)
            if (stations.at(edge.dest).district == district)
                districts.at(district).addEdge(name, edge);
    }
    priority_queue<pair<string, unsigned>, vector<pair<string, unsigned>>, LessCompare<string>> disMaxFlows;
    for (auto& [district, graph] : districts) {
        unsigned maxF = 0;
        multiset<pair<string, unsigned>, GreaterCompare<string>> orderedNodes;
        for (const auto& [name, node] : graph.nodes) {
            unsigned aux = 0;
            for (const auto& e : node.adj) aux += e.capacity;
            orderedNodes.insert({name, aux});
        }
        for (const auto& [name1, outDeg1] : orderedNodes) {
            for (const auto& [name2, outDeg2] : orderedNodes) {
                if (name1 == name2) continue;
                if (outDeg1 < maxF) continue;
                unsigned flow = graph.maxFlow(name1, name2);
                if (flow > maxF) maxF = flow;
            }
        }
        disMaxFlows.push({district, maxF});
    }
    list<pair<string, unsigned>> res;
    for (int i = 0; i < k; i++) {
        if (disMaxFlows.empty()) break;
        res.push_back(disMaxFlows.top());
        disMaxFlows.pop();
    }
    return res;
}
unsigned RailNetwork::maxFlowStation(const string &station) {
    // Exercise [2.4]
    list<string> nodesAtDistanceTwo = distancedNodes(station, 2);
    if (nodesAtDistanceTwo.empty()) {
        unsigned sum = 0;
        for (const Edge& edge : getAdj(station))
            sum += edge.capacity;
        return sum;
    }
    Node sourceNode = Node(sourceNodeName, {});
    nodes.insert({sourceNodeName, sourceNode});
    for (const string& node : nodesAtDistanceTwo) {
        addEdge(sourceNodeName, Edge(sourceNodeName, node, INVALID, UINT_MAX));
    }
    unsigned res = maxFlow(sourceNodeName, station);
    nodes.erase(sourceNodeName);
    return res;
}


unsigned RailNetwork::maxFlowMinCost(const string &origin, const string &destination) {
    // Exercise [3.1]
    // 1 - Get all paths with the minimum cost.
    // 2 - Build a sub-graph with only the nodes and edges that belong to any minCost path.
    // 3 - Calculate max flow of the sub-graph.
    // Step 1:
    list<list<string>> allPathsMinCost = BFSCost(origin, destination);
    // Step 2:
    RailNetwork subGraph;
    for (const list<string>& path : allPathsMinCost){
        string prev;
        for (const string& node : path) {
            if (!prev.empty()) {
                try {
                    subGraph.getEdge(prev, node);
                } catch (std::out_of_range& e) {
                    // If Edge not in subGraph add it.
                    subGraph.addEdge(prev, getEdge(prev, node));
                }
            }
            try {
                subGraph.getNode(node);
            } catch (std::exception& e) {
                // If Node not in subGraph add it.
                subGraph.addNode(node, {});
            }
            prev = node;
        }
    }
    // Step 3:
    return subGraph.maxFlow(origin, destination);
}

unsigned RailNetwork::maxFlowReduced(const string &origin, const string &destination) {
    clearFlow();
    unsigned maxFlow = 0;
    while(true){
        list<string> res = BFSActive(origin,destination);
        if(res.empty()) break;
        unsigned bottleneck = UINT_MAX;
        auto it = res.begin();
        auto itNext = res.begin();
        itNext++;
        while(itNext != res.end()){ // find bottleneck in the shortest path
            Node vert = nodes.at(*it);
            string next = *itNext;
            Edge edge = getEdge(*it,next);
            unsigned remaining = edge.capacity - edge.flow;
            if(remaining < bottleneck) bottleneck = remaining;
            it++; itNext++;
        }
        it = res.begin();
        itNext = res.begin();
        itNext++;
        maxFlow += bottleneck;
        while(it != res.end()){ // find bottleneck in the shortest path
            Node vert = nodes.at(*it);
            string next = *itNext;
            Edge edge = getEdge(*it,next);
            edge.flow += bottleneck;
            it++; itNext++;
        }
    }
    return maxFlow;
}

list<string> RailNetwork::topAffectedStations(int k, const unordered_map<string,Station>& stations) {
    priority_queue<pair<string, unsigned>, vector<pair<string, unsigned>>, LessCompare<string>> flowVariance;
    for(auto [name,station]:stations){
        list<string> nodesAtDistanceTwo = distancedNodes(name,2);
        if (nodesAtDistanceTwo.empty()) {
            unsigned sum = 0;
            for (const Edge& edge : getAdj(name))
                sum += edge.capacity;
        }
        Node sourceNode = Node(sourceNodeName, {});
        nodes.insert({sourceNodeName, sourceNode});
        for (const string& node : nodesAtDistanceTwo) {
            addEdge(sourceNodeName, Edge(sourceNodeName, node, INVALID, UINT_MAX));
        }
        unsigned normalFlow = maxFlow(sourceNodeName, name);
        unsigned reducedFlow = maxFlowReduced(sourceNodeName, name);
        flowVariance.emplace(name,normalFlow-reducedFlow);
        nodes.erase(sourceNodeName);
    }
    list<string> res;
    for (int i = 0; i < k; i++) {
        if (flowVariance.empty()) break;
        res.push_back(flowVariance.top().first);
        flowVariance.pop();
    }
    return res;
}
