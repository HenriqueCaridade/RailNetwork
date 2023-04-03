
#ifndef RAILNETWORK_STATION_H
#define RAILNETWORK_STATION_H

#include <string>
#include <utility>


struct Station {
    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;

    Station(std::string name, std::string district, std::string municipality, std::string township, std::string line) :
            name(std::move(name)),
            district(std::move(district)),
            municipality(std::move(municipality)),
            township(std::move(township)),
            line(std::move(line)) {};
};


#endif //RAILNETWORK_STATION_H
