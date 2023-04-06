
#ifndef RAILNETWORK_RAILMANAGER_H
#define RAILNETWORK_RAILMANAGER_H

#include <unordered_map>
#include <string>

#include "RailNetwork.h"
#include "CSVReader.h"
#include "Station.h"


class RailManager {
    std::unordered_map<std::string, Station> stations;
    std::unordered_map<std::string, std::unordered_map<std::string, Segment>> segments;
    RailNetwork railNet;

    void addSegment(const std::string &stationA, const std::string &stationB, unsigned int capacity, SegmentType service);
    void addStation(const std::string &name, const std::string &district, const std::string &municipality, const std::string &township, const std::string &line);

    void initializeStations(const CSV& stationsCSV);
    void initializeSegments(const CSV& networkCSV);
    void initializeNetwork();
    void clearData();
public:
    RailManager();
    explicit RailManager(const std::string& datasetPath);
    void initializeData(const std::string& datasetPath);

    const Segment& getSegment(const std::string& origin, const std::string& destination);
    const Station& getStation(const std::string& station);


};


#endif //RAILNETWORK_RAILMANAGER_H
