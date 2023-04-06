
#include <iostream>

#include "RailManager.h"
using namespace std;


RailManager::RailManager() = default;

RailManager::RailManager(const string& datasetPath) {
    initializeData(datasetPath);
}

void RailManager::addSegment(const string& stationA, const string& stationB, unsigned int capacity, SegmentType service) {
    segments[stationA].insert({stationB, Segment(stationA, stationB, capacity, service)});
    segments[stationB].insert({stationA, Segment(stationB, stationA, capacity, service)});
}

void RailManager::addStation(const string& name, const string& district, const string& municipality, const string& township, const string& line){
    stations.insert({name, Station(name, district, municipality, township, line)});
}

const Segment& RailManager::getSegment(const string &origin, const string &destination) {
    return segments.at(origin).at(destination);
}

const Station &RailManager::getStation(const std::string &station) {
    return stations.at(station);
}

void RailManager::initializeStations(const CSV &stationsCSV) {
    unsigned emptyCount = 0, repeatedCount = 0;
    for(int i = 1; i < stationsCSV.size(); i++){ // Skip first line
        CSVLine line = stationsCSV[i];
        if (line.size() != 5) continue;
        // Check If Is Empty Entry
        bool emptyEntry = false;
        for (const string& str : line)
            if (str.empty()) {
                emptyEntry = true;
                break;
            }
        if (emptyEntry) {
            emptyCount++;
            continue;
        }
        // Check If Already Added
        if (stations.find(line[0]) != stations.end()) {
            repeatedCount++;
            continue;
        }
        addStation(line[0], line[1], line[2], line[3], line[4]);
    }
    cout << "stations.csv Report:\nEmpty Entries: " << emptyCount << "\nRepeated Entries: " << repeatedCount << '\n' << endl;
}

void RailManager::initializeSegments(const CSV &networkCSV) {
    unsigned emptyCount = 0, repeatedCount = 0;
    for(int i = 1; i < networkCSV.size(); i++){ // Skip first line
        CSVLine line = networkCSV[i];
        if (line.size() != 4) continue;
        // Check If Is Empty Entry
        bool emptyEntry = false;
        for (const string& str : line)
            if (str.empty()) {
                emptyEntry = true;
                break;
            }
        if (emptyEntry){
            emptyCount++;
            continue;
        }
        // Check If Already Added
        if (segments.find(line[0]) != segments.end() && (segments.at(line[0]).find(line[1]) != segments.at(line[0]).end())) {
            repeatedCount++;
            continue;
        }
        SegmentType service = INVALID;
        if (line[3] == "STANDARD") service = STANDARD;
        else if (line[3] == "ALFA PENDULAR") service = ALFA_PENDULAR;
        if (service) addSegment(line[0], line[1], stoi(line[2]), service);
    }
    cout << "network.csv Report:\nEmpty Entries: " << emptyCount << "\nRepeated Entries: " << repeatedCount << '\n' << endl;
}

void RailManager::initializeNetwork() {
    for (const auto& [name, station] : stations) {
        list<RailNetwork::Edge> l;
        for (const auto& [dest, seg] : segments[name])
            l.emplace_back(name, dest, seg.service, seg.capacity);
        railNet.addNode(name, l);
    }
}

void RailManager::clearData() {
    stations.clear();
    segments.clear();
    railNet = RailNetwork();
}

void RailManager::initializeData(const string& datasetPath) {
    clearData();
    initializeStations(CSVReader::read(datasetPath + "stations.csv"));
    initializeSegments(CSVReader::read(datasetPath + "network.csv"));
    initializeNetwork();
    // string a = "Casa Branca";
    // string b = "Portalegre";
    // cout << railNet.maxFlow(a,b) << endl;
}

bool RailManager::segmentExists(const string &origin, const string &destination) {
    return (segments.find(origin) != segments.end()) && (segments.at(origin).find(destination) != segments.at(origin).end());
}

bool RailManager::stationExists(const string &station) {
    return stations.find(station) != stations.end();
}

void RailManager::reactivateAllStations() {
    for (auto& [_, node] : railNet.nodes)
        node.active = true;
}

void RailManager::reactivateAllSegments() {
    for (auto& [_, node] : railNet.nodes)
        for (auto& edge : node.adj)
            edge.active = true;
}

void RailManager::deactivateStations(const list<string> &stationsToDeactivate) {
    for (const string& station : stationsToDeactivate)
        railNet.getNode(station).active = false;
}

void RailManager::deactivateSegments(const list<pair<string, string>> &segmentsToDeactivate) {
    for (const auto& [stationA, stationB] : segmentsToDeactivate)
        railNet.getEdge(stationA, stationB).active = false;
}

unsigned RailManager::maxFlow(const string &origin, const string &destination) {
    return railNet.maxFlow(origin, destination);
}

list<pair<string, string>> RailManager::importantStations() {
    return railNet.importantStations();
}

list<string> RailManager::topMunicipalities(int k) {
    return railNet.topMunicipalities(k, stations);
}

list<string> RailManager::topDistricts(int k) {
    return railNet.topDistricts(k, stations);
}

unsigned RailManager::maxFlowStation(const string &station) {
    return railNet.maxFlowStation(station);
}

unsigned RailManager::maxFlowMinCost(const string &origin, const string &destination) {
    return railNet.maxFlowMinCost(origin, destination);
}

unsigned RailManager::maxFlowReduced(const string &origin, const string &destination, const list<pair<string, string>>& segmentsToDeactivate, const list<string>& stationsToDeactivate) {
    reactivateAllStations();
    reactivateAllSegments();
    deactivateSegments(segmentsToDeactivate);
    deactivateStations(stationsToDeactivate);
    return railNet.maxFlowReduced(origin, destination);
}

std::list<std::string> RailManager::topAffectedStations(int k, const list<std::pair<std::string, std::string>> &segmentsToDeactivate,const list<std::string> &stationsToDeactivate) {
    reactivateAllStations();
    reactivateAllSegments();
    deactivateSegments(segmentsToDeactivate);
    deactivateStations(stationsToDeactivate);
    return railNet.topAffectedStations(k,stations);
}



