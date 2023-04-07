
#ifndef RAILNETWORK_STATION_H
#define RAILNETWORK_STATION_H

#include <string>
#include <utility>

/**
*@brief Struct representing a train station.
*/
struct Station {
    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;
/**
* @brief Construct a new Station object.
* @param name The name of the station.
* @param district The district where the station is located.
* @param municipality The municipality where the station is located.
* @param township The township where the station is located.
* @param line The train line the station is part of.
*/
    Station(std::string name, std::string district, std::string municipality, std::string township, std::string line) :
            name(std::move(name)),
            district(std::move(district)),
            municipality(std::move(municipality)),
            township(std::move(township)),
            line(std::move(line)) {};
};


#endif //RAILNETWORK_STATION_H
