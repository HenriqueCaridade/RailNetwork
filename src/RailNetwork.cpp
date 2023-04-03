
#include <stdexcept>
#include "RailNetwork.h"
#include "Segment.h"

using namespace std;


void RailNetwork::addSegment(const string& origin, const string& destination, unsigned int capacity, SegmentType service) {
    edges[origin].insert({destination, Segment(origin, destination, capacity, service)});
}

const Segment* RailNetwork::getSegment(const string &origin, const string &destination) {
    try {
        return &edges.at(origin).at(destination);
    } catch (std::out_of_range& _) {
        return nullptr;
    }
}

const RailNetwork::Node *RailNetwork::getNode(const string &station) {
    try {
        return &nodes.at(station);
    } catch (std::out_of_range& _) {
        return nullptr;
    }
}

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
void RailNetwork::clearVisits() {
    for(auto& p : nodes)
        p.second.visited = false;
}
void RailNetwork::visit(const std::string& station){
    nodes[station].visited=true;
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
        for(const string& adj:adjacents){
            Node aux=nodes[adj];
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
        while(true){
            if(res.front()!=src){
                res.push_front(nodes[res.front()].prev);
            }
            else break;
        }
    }
    return res;
}

list<string> RailNetwork::getAdj(const string &station) {
    return nodes[station].adj;
}
