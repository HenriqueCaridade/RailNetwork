
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

    unsigned maxFlow(const std::string& origin, const std::string& destination);
    std::list<std::pair<std::string, std::string>> importantStations();
    std::list<std::string> topMunicipalities(int k);
    std::list<std::string> topDistricts(int k);
    unsigned maxFlowStation(const std::string& station);
    unsigned maxFlowMinCost(const std::string& origin, const std::string& destination);
    unsigned maxFlowReduced(const std::string& origin, const std::string& destination, const std::list<std::pair<std::string, std::string>>& segmentsToDeactivate, const std::list<std::string>& stationsToDeactivate);
    std::list<std::string> topAffectedStations(int k,const std::list<std::pair<std::string, std::string>>& segmentsToDeactivate, const std::list<std::string>& stationsToDeactivate);


    bool segmentExists(const std::string& origin, const std::string& destination);
    bool stationExists(const std::string& station);

    void reactivateAllStations();
    void reactivateAllSegments();
    void deactivateStations(const std::list<std::string>& stations);
    void deactivateSegments(const std::list<std::pair<std::string, std::string>>& segments);

    friend class App;

};


#endif //RAILNETWORK_RAILMANAGER_H
