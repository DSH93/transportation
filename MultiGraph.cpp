#include <fstream>
#include <sstream>
#include <iostream>
#include "MultiGraph.h"


MultiGraph::MultiGraph() : outputFile("output.dat"), size(0), inter() {
    //default values, update by the config.dat
    waitingTimes["bus"] = 2;
    waitingTimes["tram"] = 3;
    waitingTimes["sprinter"] = 4;
    waitingTimes["rail"] = 5;
    waitingTimes["intercity"] = 15;
    waitingTimes["stad"] = 10;
    waitingTimes["centraal"] = 5;
    vehicleType = {"bus", "tram", "sprinter", "rail"};
}

MultiGraph::~MultiGraph() = default;

bool MultiGraph::executeSingleDoubleArgCommands(const std::string& command, const std::vector<std::string>& tokens) {
    if ((command == "load" || command == "outbound" || command == "inbound") && tokens.size() == 2) {
        if (command == "load") load(tokens[1]);
        else if (command == "outbound") outboundBFS(tokens[1], false);
        else if (command == "inbound") inboundTransposedBFS(tokens[1]);
        return true;
    } else if (command == "print" && tokens.size() == 1) {
        printMultiGraph();
        return true;
    }
    return false;
}

bool MultiGraph::executeExpressCommands(const std::string& command, const std::vector<std::string>& tokens) {
    if ((command == "uniExpress" && tokens.size() == 3) || (command == "multiExpress" && tokens.size() == 3) ||
        (command == "viaExpress" && tokens.size() == 4)) {
        if (command == "uniExpress") uniExpressDijkstra(tokens[2], tokens[1], true, false);
        else if (command == "multiExpress") multiExpressDijkstra(tokens[1], tokens[2], false);
        else if (command == "viaExpress") viaExpress(tokens[1], tokens[2], tokens[3]);
        return true;
    }
    return false;
}

void MultiGraph::start() {
    while (true) {
        auto tokens = interaction::userInteraction();
        std::string choice = tokens[0];
        if (choice == "exit" && tokens.size() == 1) break;
        bool executed = executeSingleDoubleArgCommands(choice, tokens) || executeExpressCommands(choice, tokens);
        if (!executed) interaction::exception("ERROR: The operation is not valid. ");
    }
}


void MultiGraph::load(const std::string &file_name) {
    if(readInput(file_name)) std::cout << "Update was successful." << std::endl;

}

void MultiGraph::outboundBFS(const std::string &src, bool isTranspose) {
    for (const auto &vehicle: vehicleType) {
        initializeValues();
        if (stations.find(src) != stations.end()) {
            if (isTranspose) stations[src]->getTransNeighbors(vehicle);
            else stations[src]->getNeighbors(0, std::string(), false, std::string(), 0, vehicle, true, src);
        } else {
            interaction::notExistExceptions(src);
            return;
        }

        interaction::countingSpaces(vehicle);
        auto st = checkDistanceValues(src);
        interaction::prepareBeforePrinting(st);
    }
}

void MultiGraph::inboundTransposedBFS(const std::string &src) {
    outboundBFS(src, true);
}

bool MultiGraph::checkStationExists(const std::string& station) {
    if (stations.find(station) == stations.end()) {
        interaction::notExistExceptions(station);
        return false;
    }
    return true;
}


int MultiGraph::uniExpressDijkstra(const std::string &dest_station, const std::string &src_station, bool isLimited,
                                   bool via) {

    if (!checkStationExists(dest_station) || !checkStationExists(src_station)) return 0;
    std::vector<std::string> backTrackStations;
    int time;
    std::string backing;

    if (!isLimited) {
        initializeValues();
        backTrackStations.clear();
        stations[src_station]->getNeighbors(0, dest_station, true, src_station, 0, "any", isLimited, std::string());
        time = stations[dest_station]->get_d();
        backing = stations[dest_station]->getFatherStr();
        backTrackStations.push_back(backing);
        while (!backing.empty()) {
            backing = stations[backing]->getFatherStr();
            backTrackStations.push_back(backing);
        }

        if (via) return time;
        std::string sourceStation = stations[src_station]->getStationType() + stations[src_station]->getStationName();
        std::string destStation = stations[dest_station]->getStationType() + stations[dest_station]->getStationName();
        interaction::printMultiExpress(time, sourceStation, destStation, backTrackStations);
        return 0;
    }
    for (const auto &vehicle: vehicleType) {
        initializeValues();
        backTrackStations.clear();
        stations[src_station]->getNeighbors(0, dest_station, true, src_station, 0, vehicle, isLimited, src_station);
        time = stations[dest_station]->get_d();
        interaction::countingSpaces(vehicle);
        interaction::printUniExpress(time);
    }
    return 0;
}

int MultiGraph::multiExpressDijkstra(const std::string &src_station, const std::string &dist_station, bool isVia) {
    return uniExpressDijkstra(dist_station, src_station, false, isVia);
}

void MultiGraph::viaExpress(const std::string &src_station, const std::string &via,
                            const std::string &dist_station) {
    if(!isStationExists(src_station)) {
        interaction::notExistExceptions(src_station);
        return;
    }
    if(!isStationExists(via)) {
        interaction::notExistExceptions(via);
        return;
    }
    if(!isStationExists(dist_station)) {
        interaction::notExistExceptions(dist_station);
        return;
    }

    int until = multiExpressDijkstra(src_station, via, true);
    int after = multiExpressDijkstra(via, dist_station, true);

    std::string srcStationAndType = stations[src_station]->getStationType();
    std::string viaStationAndType = stations[via]->getStationType();
    std::string destStationAndType = stations[src_station]->getStationType();

    viaStationAndType.append(via);
    srcStationAndType.append(src_station);
    destStationAndType.append(dist_station);
    interaction::printViaExpress(srcStationAndType, viaStationAndType, destStationAndType, until, after);
}

void MultiGraph::printMultiGraph() {
    std::vector<std::vector<std::string>> multiGraph;
    for (const auto &vehicle: vehicleType) {
        std::vector<std::string> links;
        links.push_back(vehicle);
        for (const auto &station: stations) {
            std::string fromStation = station.first;
            std::string type = station.second->getStationType();
            if (type != "stad") fromStation = type + fromStation;
            links.push_back(fromStation);

            for (int i = 0; i < station.second->getLinks().size(); ++i) {
                if (station.second->getLinks()[i].second->get_vehicle() == vehicle) {
                    std::string stationStr = station.second->getLinks()[i].first->getStationName();
                    std::string typeStr = station.second->getLinks()[i].first->getStationType();
                    if (typeStr != "stad") stationStr = typeStr + stationStr;
                    std::string edgeWeight = std::to_string(station.second->getLinks()[i].second->get_duration());
                    edgeWeight.append(" ");
                    edgeWeight.append(stationStr);
                    links.push_back(edgeWeight);
                } else continue;
            }
            multiGraph.push_back(links);
            links.clear();
        }
    }
    interaction::printMultiGraph(multiGraph, outputFile);
}

std::string processPrefix(std::string& station) {
    std::string prefix = station.substr(0, 2);
    if (prefix == "IC" || prefix == "CS") {
        station = station.substr(2);
        return prefix;
    }
    return "";
}


bool MultiGraph::readInput(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()){
        interaction::fileNotExceptions(file_name);
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        if (file_name == "config.dat") {
            std::string first, second;
            if (iss >> first >> second) {
                waitingTimes[first] = std::stoi(second);
            }
        } else {
            std::string vehicle = file_name;
            vehicle.erase(vehicle.size() - 5);
            std::string source, dist, weightStr;
            int weight;

            if (iss >> source >> dist >> weightStr) {
                weight = std::stoi(weightStr);
                std::string prefix_src = processPrefix(source);
                std::string prefix_dist = processPrefix(dist);
                linkStationsAndEdges(source, prefix_src, dist, prefix_dist, weight, vehicle);
            }
        }
    }
    file.close();
    return true;
}

void updateChangeTimeAndType(const std::string& prefix, int& changeTime, std::string& stationType, const std::map<std::string, int>& waiting_times) {
    if (prefix == "CS") {
        changeTime = waiting_times.at("centraal");
        stationType = prefix;
    } else if (prefix == "IC") {
        changeTime = waiting_times.at("intercity");
        stationType = prefix;
    }
}


void MultiGraph::linkStationsAndEdges(const std::string &sourceString, const std::string &src_prefix,
                                      const std::string &destinationString, const std::string &prefix_dist, int weight,
                                      const std::string &vehicle) {
    int changeTimeSrc = waitingTimes["stad"];
    int changeTimeDist = waitingTimes["stad"];
    std::string stationSrcType = "stad";
    std::string stationDistType = "stad";

    updateChangeTimeAndType(src_prefix, changeTimeSrc, stationSrcType, waitingTimes);
    updateChangeTimeAndType(prefix_dist, changeTimeDist, stationDistType, waitingTimes);

    int standbyTime = waitingTimes[vehicle];
    auto e = std::make_shared<Edge>(weight, vehicle, standbyTime, sourceString, destinationString);
    auto toFatherEdge = std::make_shared<Edge>(weight, vehicle, standbyTime, destinationString, sourceString);

    if (!isStationExists(destinationString)) { // if the key destinationString don't exist add new one
        auto dest = std::make_shared<Station>(destinationString, changeTimeDist, stationDistType);
        addStationMultiGraph(destinationString, dest);
    }
    // here I am checking if the key already exists if so, add to it.
    if (isStationExists(sourceString)) {
        stations[sourceString]->addStation(stations[destinationString], e);
        stations[destinationString]->addStationTransposeGraph(stations[sourceString],
                                                              toFatherEdge); // adding the transposed edge to the graph

        addEdgeMultiGraph(e);
        addTransposeEdgeMultiGraph(toFatherEdge);

    } else { //  otherwise create it and add.

        auto src = std::make_shared<Station>(sourceString, changeTimeSrc, stationSrcType);
        addStationMultiGraph(sourceString, src);

        src->addStation(stations[destinationString], e);
        stations[destinationString]->addStationTransposeGraph(stations[sourceString],
                                                              toFatherEdge); // adding the opposite edge to the graph

        addEdgeMultiGraph(e);
        addTransposeEdgeMultiGraph(toFatherEdge);
    }

}

int MultiGraph::addStationMultiGraph(const std::string &station, const std::shared_ptr<Station> &st) {
    stations[station] = st;
    return 0;
}

int MultiGraph::addTransposeEdgeMultiGraph(const std::shared_ptr<Edge> &edge) {
    route.push_back(edge);
    return 0;
}


int MultiGraph::addEdgeMultiGraph(const std::shared_ptr<Edge> &edge) {
    route.push_back(edge);
    return 0;
}

std::vector<std::pair<std::string, std::string>> MultiGraph::checkDistanceValues(const std::string &src) {
    std::vector<std::pair<std::string, std::string>> st;
    for (const auto &pair: stations) {
        std::string type = pair.second->getStationType();
        if (pair.second->get_d() != -1 && pair.second->getStationName() != src)
            st.emplace_back(pair.second->getStationType(), pair.second->getStationName());
    }
    return st;
}


bool MultiGraph::isStationExists(const std::string& station) {
    auto it = stations.find(station);
    if (it != stations.end()) return true;
    return false;
}

void MultiGraph::initializeValues() {
    for (const auto &pair: stations) {
        pair.second->set_d(-1);
        pair.second->sortStations();
        pair.second->clearEdges();
        pair.second->setFatherStr("");
    }
}

