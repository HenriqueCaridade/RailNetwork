
#include <iostream>

#include "RailManager.h"
using namespace std;


RailManager::RailManager() = default;

RailManager::RailManager(const string& datasetPath) {
    initializeStations(CSVReader::read(datasetPath + "stations.csv"));
    initializeSegments(CSVReader::read(datasetPath + "network.csv"));
    initializeNetwork();
    string a ="Casa Branca";
    string b = "Portalegre";
    cout<<railNet.maxFlow(a,b)<<endl;
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

