
#ifndef RAILNETWORK_RAILMANAGER_H
#define RAILNETWORK_RAILMANAGER_H

#include <unordered_map>
#include <string>

#include "RailNetwork.h"
#include "CSVReader.h"
#include "Station.h"


class RailManager {
    const static std::string path;
    std::unordered_map<std::string, Station> stations;
    RailNetwork railNet;

    void initializeStations(const CSV& stationsCSV);
    void initializeNetwork(const CSV& networkCSV);
public:
    RailManager();
};


#endif //RAILNETWORK_RAILMANAGER_H
