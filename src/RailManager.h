
#ifndef RAILNETWORK_RAILMANAGER_H
#define RAILNETWORK_RAILMANAGER_H

#include <unordered_map>
#include <string>

#include "RailNetwork.h"
#include "CSVReader.h"
#include "Station.h"

/**
*@brief A class representing the rail network manager.
*This class manages a rail network, composed of stations and segments connecting them. It provides various methods
*to query information about the network, such as maximum flow, top municipalities, top districts, etc. It also
*provides methods to activate and deactivate stations and segments, allowing for simulating maintenance or damage to
*the network.
*/
class RailManager {
    std::unordered_map<std::string, Station> stations;
    std::unordered_map<std::string, std::unordered_map<std::string, Segment>> segments;
    RailNetwork railNet;
/**
*
*@brief Add a new segment to the network.
*This method adds a new segment to the network connecting two stations, with a given capacity and service type.
*@param stationA The name of the first station.
*@param stationB The name of the second station.
*@param capacity The capacity of the new segment.
*@param service The service type of the new segment.
*/
    void addSegment(const std::string &stationA, const std::string &stationB, unsigned int capacity, SegmentType service);
    /**
*@brief Add a new station to the network.
*This method adds a new station to the network with a given name, district, municipality, township, and line.
*@param name The name of the new station.
*@param district The district where the new station is located.
*@param municipality The municipality where the new station is located.
*@param township The township where the new station is located.
*@param line The line to which the new station belongs.
*/
    void addStation(const std::string &name, const std::string &district, const std::string &municipality, const std::string &township, const std::string &line);
/**
*@brief Initialize the stations in the network.
*This method reads a CSV file containing station data and initializes the collection of stations in the network.
*@param stationsCSV The CSV object containing the station data.
*/
    void initializeStations(const CSV& stationsCSV);
/**
*@brief Initialize the segments in the network.
*This method reads a CSV file containing segment data and initializes the collection of segments in the network.
*@param networkCSV The CSV object containing the segment data.
*/
    void initializeSegments(const CSV& networkCSV);
/**
*@brief Initialize the network graph.
*This method initializes the rail network object that represents the network as a graph.
*/
    void initializeNetwork();
    /**
*@brief Clear all data in the network.
*This method clears all data in the network, including stations and segments.
*/
    void clearData();

public:
    /**
 * @brief Default constructor.
 *
 * Creates a new empty rail network manager.
 */
    RailManager();
    /**
 * @brief Constructs a new RailManager object and initializes it with data from a CSV file.
 * @param datasetPath The path to the directory containing the CSV files.
 */
    explicit RailManager(const std::string& datasetPath);
    /**
 * @brief Initializes the RailManager object with data from a CSV file.
 * @param datasetPath The path to the directory containing the CSV files.
 */
    void initializeData(const std::string& datasetPath);

/**
 * @brief Gets the segment between two stations.
 * @param origin The name of the origin station.
 * @param destination The name of the destination station.
 * @return The segment object between the two stations.
 */
    const Segment& getSegment(const std::string& origin, const std::string& destination);
/**
 * @brief Gets the station with the given name.
 * @param station The name of the station.
 * @return The station object.
 */
    const Station& getStation(const std::string& station);
/**
 * @brief Calculates the maximum flow between two stations.
 * @param origin The name of the origin station.
 * @param destination The name of the destination station.
 * @return The maximum flow between the two stations.
 */
    unsigned maxFlow(const std::string& origin, const std::string& destination);
    /**
 * @brief Gets a list of the most important stations in the network, in order of importance.
 * @return A list of pairs of station names and their corresponding importance values.
 */
    std::list<std::pair<std::string, std::string>> importantStations();
    /**
 * @brief Gets a list of the top k municipalities in the network, ranked by the number of stations they contain.
 * @param k The number of municipalities to include in the list.
 * @return A list of the names of the top k municipalities.
 */
    std::list<std::string> topMunicipalities(int k);
    /**
*@brief Gets the top k districts with the most stations.
*@param k The number of top districts to return.
*@return A list of the names of the top k districts.
*/
    std::list<std::string> topDistricts(int k);
    /**
*@brief Computes the maximum flow that can pass through a given station.
*@param station The name of the station.
*@return The maximum flow that can pass through the given station.
*/
    unsigned maxFlowStation(const std::string& station);
    /**
*@brief Computes the maximum flow between two stations with minimum cost.
*@param origin The name of the origin station.
*@param destination The name of the destination station.
*@return The maximum flow that can pass through the two stations with minimum cost.
*/
    unsigned maxFlowMinCost(const std::string& origin, const std::string& destination);
/**
*@brief Computes the maximum flow between two stations with some segments and/or stations deactivated.
*@param origin The name of the origin station.
*@param destination The name of the destination station.
*@param segmentsToDeactivate A list of pairs of stations that represent the segments to deactivate.
*@param stationsToDeactivate A list of names of stations to deactivate.
*@return The maximum flow that can pass through the two stations with the given segments and/or stations deactivated.
*/
    unsigned maxFlowReduced(const std::string& origin, const std::string& destination, const std::list<std::pair<std::string, std::string>>& segmentsToDeactivate, const std::list<std::string>& stationsToDeactivate);
    /**
*@brief Gets the top k stations that are most affected by the given segments and/or stations being deactivated.
*@param k The number of top affected stations to return.
*@param segmentsToDeactivate A list of pairs of stations that represent the segments to deactivate.
*@param stationsToDeactivate A list of names of stations to deactivate.
*@return A list of the names of the top k stations that are most affected by the given segments and/or stations being deactivated.
*/
    std::list<std::string> topAffectedStations(int k,const std::list<std::pair<std::string, std::string>>& segmentsToDeactivate, const std::list<std::string>& stationsToDeactivate);

/**
*@brief Checks if a segment exists between two stations.
*@param origin The name of the origin station.
*@param destination The name of the destination station.
*@return True if a segment exists between the two stations, false otherwise.
*/
    bool segmentExists(const std::string& origin, const std::string& destination);
    /**
 *@brief Checks if a station exists.
 *@param station The name of the station.
 *@return True if the station exists, false otherwise.
 */
    bool stationExists(const std::string& station);
/**
*@brief Reactivates all stations that have been deactivated.
*/
    void reactivateAllStations();
    /**
*@brief Reactivates all segments that have been deactivated.
*/
    void reactivateAllSegments();
    /**
*@brief Deactivates the given list of stations.
*@param stations A list of names of stations to deactivate.
*/
    void deactivateStations(const std::list<std::string>& stations);
    /**
 *@brief Deactivates the given list of segments.
 *@param segments A list of pairs of stations that represent the segments to deactivate.
 */
    void deactivateSegments(const std::list<std::pair<std::string, std::string>>& segments);
/**
*@brief A friend class used for testing purposes only.
*/
    friend class App;

};


#endif //RAILNETWORK_RAILMANAGER_H
