//
// Created by Dor Shukrun on 24/02/2024.
//

#ifndef HW02ADV_MULTIGRAPH_H
#define HW02ADV_MULTIGRAPH_H

#include <string>
#include <map>
#include <unordered_map>
#include "Edge.h"
#include "Station.h"
#include "interaction.h"


class MultiGraph {
    std::unordered_map<std::string, std::shared_ptr<Station>> stations;
    std::vector<std::shared_ptr<Edge>> route;
    std::vector<std::shared_ptr<Edge>> transposedRoute;
    std::vector<std::string> vehicleType;
    std::string outputFile;
    int size;
    std::map<std::string, int> waitingTimes;
    interaction inter;



public:
    MultiGraph();

    ~MultiGraph();

    bool readInput(const std::string &file_name);

    int addStationMultiGraph(const std::string &station, const std::shared_ptr<Station> &st);

    int addEdgeMultiGraph(const std::shared_ptr<Edge> &edge);

    int addTransposeEdgeMultiGraph(const std::shared_ptr<Edge> &edge);

    void outboundBFS(const std::string &src, bool isTranspose);

    void inboundTransposedBFS(const std::string &src);

    void load(const std::string &file_name);

    void update_values(int val);

    void get_input_from_user();

    int uniExpressDijkstra(const std::string &dest_station, const std::string &src_station, bool isLimited,
                           bool via);

    int multiExpressDijkstra(const std::string &src_station, const std::string &dist_station, bool isVia);

    void viaExpress(const std::string &src_station, const std::string &via,
                    const std::string &dist_station);

    void linkStationsAndEdges(const std::string &sourceString, const std::string &src_prefix,
                              const std::string &destinationString,
                              const std::string &dist, int weight, const std::string &vehicle);

    void printMultiGraph();

    std::vector<std::pair<std::string, std::string>> checkDistanceValues(const std::string &src);

    void initializeValues();

    bool checkStationExists(const std::string& station);

    void start();

    bool executeExpressCommands(const std::string& command, const std::vector<std::string>& tokens);

    bool isStationExists(const std::string& station);

    bool executeSingleDoubleArgCommands(const std::string& command, const std::vector<std::string>& tokens);

};


#endif //HW02ADV_MULTIGRAPH_H
