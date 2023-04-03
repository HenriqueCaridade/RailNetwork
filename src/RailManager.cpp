
#include "RailManager.h"

using namespace std;


const std::string RailManager::path = "../dataset";

RailManager::RailManager() {
    initializeStations(CSVReader::read(path + "stations.csv"));
}


void RailManager::initializeStations(const CSV &stationsCSV) {
    for(int i = 1; i < stationsCSV.size(); i++){ // Skip first line
        CSVLine line = stationsCSV[i];
        if(line.size() != 5) throw std::exception();
        stations.insert({line[0], Station(line[0], line[1], line[2], line[3], line[4])});
    }
}

void RailManager::initializeNetwork(const CSV &networkCSV) {
    for(int i = 1; i < networkCSV.size(); i++){ // Skip first line
        CSVLine line = networkCSV[i];
        if(line.size() != 5) throw std::exception();
        SegmentType service;
        if (line[3] == "STANDARD") service = STANDARD;
        else if (line[3] == "ALFA PENDULAR") service = ALFA_PENDULAR;
        else service = INVALID;
        railNet.addSegment(line[0], line[1], stoi(line[2]), service);
    }
}

