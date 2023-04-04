
#include "RailNetwork.h"
#include "Segment.h"

using namespace std;


const RailNetwork::Node& RailNetwork::getNode(const string &station) {
    return nodes.at(station);
}

void RailNetwork::addNode(const std::string& name, const std::list<Edge>& adj) {
    nodes.insert({name, Node(name, "", adj, false)});
}

void RailNetwork::clearVisits() {
    for(auto& p : nodes)
        p.second.visited = false;
}
void RailNetwork::visit(const std::string& station){
    nodes.at(station).visited = true;
}

list<string> RailNetwork::BFS(const string& src, const string& dest){
    clearVisits();
    queue<string> q;
    q.push(src);
    list<string> res;
    visit(src);
    bool found = false;
    while(!q.empty() ){ // No more Nodes
        string curr = q.front();
        visit(curr);
        list<Edge> edges = getAdj(curr);
        q.pop();
        for(const Edge& edge : edges) {
            Node aux = nodes.at(edge.dest);
            if(!aux.visited){
                aux.prev = curr;
                q.push(edge.dest);
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
list<std::string> RailNetwork::BFSFlow(const string &src, const string &dest) {
    clearVisits();
    queue<string> q;
    q.push(src);
    list<string> res;
    visit(src);
    bool found = false;
    while(!q.empty() ){ // No more Nodes
        string curr = q.front();
        visit(curr);
        list<Edge> edges = getAdj(curr);
        q.pop();
        for(const Edge& edge : edges) {
            if(edge.flow == edge.capacity) continue; // if segment flow is full dont add node to queue
            Node aux = nodes.at(edge.dest);
            if(!aux.visited){
                aux.prev = curr;
                q.push(edge.dest);
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
            int flow = maxFlow(name1, name2);
            if (flow > maxF) pairs = list<pair<string, string>>{{name1, name2}};
            else if (flow == maxF) pairs.emplace_back(name1, name2);
        }
    }
    return pairs;
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

RailNetwork::Edge &RailNetwork::getEdge(const std::string src, const string &dest) {
    for(Edge& edge:nodes.at(src).adj){
        if(edge.dest==dest)
            return edge;
    }
}

void RailNetwork::clearFlow() {
    for(auto& [name,node]:nodes){
        for(Edge& e:node.adj)
            e.flow=0;
    }
}


