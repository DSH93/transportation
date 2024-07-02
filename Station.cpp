//
// Created by Dor Shukrun on 24/02/2024.
//

#include <iostream>
#include <algorithm>
#include <utility>
#include "Station.h"


std::set<std::shared_ptr<Edge>> Station::edges;


Station::Station(const Station& other)
        : station_name(other.station_name), transChangeTime(other.transChangeTime),
          links(other.links), transposeGraph(other.transposeGraph),
          station_type(other.station_type), d(other.d), fatherStr(other.fatherStr) {}


Station& Station::operator=(const Station& other) {
    if (this != &other) {
        station_name = other.station_name;
        transChangeTime = other.transChangeTime;
        links = other.links;
        transposeGraph = other.transposeGraph;
        station_type = other.station_type;
        d = other.d;
        fatherStr = other.fatherStr;
    }
    return *this;
}


Station::Station(Station&& other) noexcept
        : station_name(std::move(other.station_name)), transChangeTime(other.transChangeTime),
          links(std::move(other.links)), transposeGraph(std::move(other.transposeGraph)),
          station_type(std::move(other.station_type)), d(other.d), fatherStr(std::move(other.fatherStr)) {
}


Station& Station::operator=(Station&& other) noexcept {
    if (this != &other) { // self-assignment check
        station_name = std::move(other.station_name);
        transChangeTime = other.transChangeTime;
        links = std::move(other.links);
        transposeGraph = std::move(other.transposeGraph);
        station_type = std::move(other.station_type);
        d = other.d;
        fatherStr = std::move(other.fatherStr);
    }
    return *this;
}


Station::~Station() = default;


Station::Station(std::string stationName, int ChangeTime, std::string station_type) : station_name(std::move(stationName)),
        transChangeTime(ChangeTime),
        station_type(std::move(station_type)),
        d(0), fatherStr(std::string()) {}

void Station::addStation(const std::shared_ptr<Station> &dest, const std::shared_ptr<Edge> &roth) {
    links.emplace_back(dest, roth);
}

void Station::addStationTransposeGraph(const std::shared_ptr<Station> &father, const std::shared_ptr<Edge> &roth) {
    transposeGraph.emplace_back(father, roth);
}


int Station::get_d() const { return d; }

void Station::set_d(int d_value) { this->d = d_value; }

void Station::getTransNeighbors(const std::string &vehicle) {
    if (this->d != -1) return;
    for (auto &i: transposeGraph) {
        if (i.second->get_vehicle() == vehicle) {
            i.first->set_d(0);
            i.first->getTransNeighbors(vehicle);
        }
    }
}

void Station::getNeighbors(int lastWeight, const std::string &dest, bool isDijkstra,
                           const std::string &src, int currentEdge,
                           const std::string &vehicle, bool isLimited, const std::string &last) {

    if (this->d != -1 && !isDijkstra) return; // for the bfs return if already visited.
    if (!isDijkstra) set_d(0); // for the bfs, mark as visited by d = 0

    if (isDijkstra) { // if we want to look for the shorter path
        if (this->getStationName() == src && d == -1) {
            set_d(0); // d = 0 for the first station.
            this->setFatherStr("");
        }
        if (this->getStationName() == src && lastWeight > 0) return; // if we get into a circle.
        if (this->getStationName() != src && d == -1) {
            set_d(lastWeight); // for the first time that we are visiting new station.
            setFatherStr(last);
        }
        if (lastWeight < d) {
            set_d(lastWeight); // if we find a shorter path into the solution update the d value.
            this->setFatherStr(last);
        }
        if (this->getStationName() == dest)return; // if we find a solution return.
    }

    bool hasChange = false;
    for (const auto &pair: links) {
        if (isLimited) { // for the uniExpress case
            if (vehicle == pair.second->get_vehicle() &&
                pair.first->getStationName() != src) { // we want to be sure that we are avoiding circles.
                if (isVisit(pair.second)) continue;
                edges.insert(pair.second);
                currentEdge = pair.second->getWeight();
                lastWeight = currentEdge + d;
                pair.first->getNeighbors(lastWeight, dest, isDijkstra, src, 0, vehicle, true, this->getStationName());
            }

        } else { // for the multiExpress case
            std::string changeVehicle = vehicle;
            if (pair.first->getStationName() != src) { // we want to be sure that we are avoiding circles.
                if (isVisit(pair.second)) continue;
                edges.insert(pair.second);
                if (changeVehicle != pair.second->get_vehicle()) hasChange = true;
                if (vehicle == "any") hasChange = false;
                currentEdge = pair.second->get_duration();
                if (hasChange) currentEdge = pair.second->get_duration() + pair.first->getStationChangeTime();
                changeVehicle = pair.second->get_vehicle();
                lastWeight = currentEdge + d;
                pair.first->getNeighbors(lastWeight, dest, isDijkstra, src, currentEdge, changeVehicle, false,
                                         this->getStationName());
            }
        }
    }
}

std::string Station::getStationName() const { return this->station_name; }

std::string Station::getStationType() const {
    return this->station_type;
}

bool compareEdges(const std::pair<std::shared_ptr<Station>, std::shared_ptr<Edge>> &a,
                  const std::pair<std::shared_ptr<Station>, std::shared_ptr<Edge>> &b) {
    return a.second->getWeight() < b.second->getWeight();
}

void Station::sortStations() { std::sort(links.begin(), links.end(), compareEdges); }

bool Station::isVisit(const std::shared_ptr<Edge> &edge) {
    unsigned int beforeAdding = edges.size();
    edges.insert(edge);
    unsigned int afterAdding = edges.size();
    return beforeAdding == afterAdding; // that means nothing has change and the edge is already in our set.
}

void Station::clearEdges() { edges.clear(); }

std::vector<std::pair<std::shared_ptr<Station>, std::shared_ptr<Edge>>> Station::getLinks() const { return links; }

int Station::getStationChangeTime() const { return this->transChangeTime; }

std::string Station::getFatherStr() const { return this->fatherStr; }

void Station::setFatherStr(const std::string &father) { this->fatherStr = father; }
