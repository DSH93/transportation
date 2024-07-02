//
// Created by Dor Shukrun on 24/02/2024.
//

#include <iostream>
#include <utility>
#include "Edge.h"

// int route_duration, standby;
//    std::string vehicle;
//    std::string src;
//    std::string dist;


Edge::Edge() :
        route_duration(0),
        standby(0),
        vehicle("0"),
        src("source"),
        dist("dist") {}

Edge::Edge(int route_duration, std::string vehicle, int standby, std::string src, std::string dist) :
        route_duration(route_duration),
        standby(standby),
        vehicle(std::move(vehicle)),
        src(std::move(src)),
        dist(std::move(dist)) {}

void Edge::update_edge(int rdur, std::string vic, int stby) {
    this->route_duration = rdur;
    this->vehicle = std::move(vic);
    this->standby = stby;
}

int Edge::get_duration() const {
    return this->route_duration;
}

std::string Edge::get_vehicle() {
    return this->vehicle;
}

int Edge::get_standby() const {
    return this->standby;
}

void Edge::set_duration(int dur) {

}

void Edge::set_vehicle(const std::string& vic) {

}

void Edge::set_standby(int stby) {

}

int Edge::getWeight() const {
    return route_duration + standby;
}

std::string Edge::getDest() const {
    return this->dist;
}

std::string Edge::getSrc() const {
    return this->src;
}
