
#include "RailNetwork.h"
#include "Segment.h"

using namespace std;


const RailNetwork::Node& RailNetwork::getNode(const string &station) {
    return nodes.at(station);
}

void RailNetwork::addNode(const std::string& name, const std::list<std::string>& adj) {
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
        list<string> adjacents = getAdj(curr);
        q.pop();
        for(const string& adj : adjacents){
            Node aux = nodes.at(adj);
            if(!aux.visited){
                aux.prev = curr;
                q.push(adj);
            }
            if(adj == dest){
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

list<string> RailNetwork::getAdj(const string &station) {
    return nodes.at(station).adj;
}

// =============================================== //
// ||          ALGORITHMIC FUNCTIONS            || //
// =============================================== //

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