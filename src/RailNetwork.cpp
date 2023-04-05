
#include <unordered_set>
#include <queue>
#include <algorithm>

#include "RailNetwork.h"
#include "Segment.h"

using namespace std;


const RailNetwork::Node& RailNetwork::getNode(const string &station) {
    return nodes.at(station);
}

void RailNetwork::addNode(const std::string& name, const std::list<Edge>& adj) {
    nodes.insert({name, Node(name, "", adj, false)});
}

RailNetwork::Edge &RailNetwork::getEdge(const std::string& src, const string &dest) {
    for(Edge& edge:nodes.at(src).adj)
        if(edge.dest == dest)
            return edge;
}

void RailNetwork::clearFlow() {
    for(auto& [name,node] : nodes)
        for(Edge& e : node.adj)
            e.flow = 0;
}

void RailNetwork::clearVisits() {
    for(auto& p : nodes)
        p.second.visited = false;
}
void RailNetwork::visit(const std::string& station){
    nodes.at(station).visited = true;
}

list<std::string> RailNetwork::BFSFlow(const string &src, const string &dest) {
    clearVisits();
    queue<pair<string, SegmentType>> q;
    q.push({src, INVALID});
    list<string> res;
    visit(src);
    bool found = false;
    while(!q.empty() ){ // No more Nodes
        const auto& [curr, type] = q.front();
        visit(curr);
        list<Edge> edges = getAdj(curr);
        q.pop();
        for(const Edge& edge : edges) {
            if (!type && (type != edge.type)) continue; // Different Train
            if (edge.flow == edge.capacity) continue; // if segment flow is full dont add node to queue
            Node& aux = nodes.at(edge.dest);
            if(!aux.visited){
                aux.prev = curr;
                q.push({edge.dest, type});
            }
            if(edge.dest == dest){
                found = true;
                break;
            }
        }
        if(found) break;
    }
    if(found){
        res.push_front(dest);
        while(true) {
            if (res.front() != src) {
                res.push_front(nodes.at(res.front()).prev);
            }
            else break;
        }
    }
    return res;
}
list<RailNetwork::Edge> RailNetwork::getAdj(const string &station) {
    return nodes.at(station).adj;
}

void RailNetwork::addEdge(const string &node, const Edge &edge) {
    nodes.at(node).adj.push_back(edge);
}

class ComparePairs {
public:
    bool operator() (const pair<string,unsigned>& p1, const pair<string, unsigned>& p2){
        return p1.second > p2.second;
    }
};

// []===========================================[] //
// ||          ALGORITHMIC FUNCTIONS            || //
// []===========================================[] //

unsigned RailNetwork::maxFlow(const string &origin, const string &destination) {
    // Exercise [2.1]
    clearFlow();
    unsigned maxFlow = 0;
    while(true){
        list<string> res = BFSFlow(origin,destination);
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

list<pair<string,string>> RailNetwork::importantStations() {
    // Exercise [2.2]
    int maxF = 0;
    list<pair<string, string>> pairs;
    for (const auto& [name1, _1] : nodes) {
        for (const auto& [name2, _2] : nodes) {
            if (name1 == name2) continue;
            unsigned flow = maxFlow(name1, name2);
            if (flow > maxF) pairs = list<pair<string, string>>{{name1, name2}};
            else if (flow == maxF) pairs.emplace_back(name1, name2);
        }
    }
    return pairs;
}

list<string> RailNetwork::topMunicipalities(int k, const unordered_map<string, Station>& stations) {
    // Exercise [2.3]
    // Where should management assign larger budgets?
    // Ans: To municipalities where there are more trains (Max Flow).
    unordered_map<string, RailNetwork> municipalities;
    for (auto& [name, node] : nodes) {
        const string& municipality = stations.at(name).municipality;
        municipalities[municipality].addNode(name, {});
        for (const auto& edge : node.adj)
            if (stations.at(edge.dest).municipality == municipality)
                addEdge(name, edge);
    }
    priority_queue<pair<string, unsigned>, vector<pair<string, unsigned>>, ComparePairs> munMaxFlows;
    for (auto& [municipality, graph] : municipalities) {
        priority_queue<pair<string, unsigned>, vector<pair<string, unsigned>>, ComparePairs> nodeDegrees;
        for (auto& [name, node] : graph.nodes) {
            unsigned degree = 0;
            for (auto& edge : node.adj)
                degree += edge.capacity;
            nodeDegrees.push({name, degree});
        }
        string origin = nodeDegrees.top().first;
        nodeDegrees.pop();
        string dest = nodeDegrees.top().first;
        unsigned maxFlow = graph.maxFlow(origin, dest);
        munMaxFlows.push({municipality, maxFlow});
    }
    list<string> res;
    for (int i = 0; i < k; i++) {
        if (munMaxFlows.empty()) break;
        res.push_back(munMaxFlows.top().first);
        munMaxFlows.pop();
    }
    return res;
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





