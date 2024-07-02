//
// Created by Dor Shukrun on 24/02/2024.
//

#ifndef HW02ADV_EDGE_H
#define HW02ADV_EDGE_H


class Edge {
    int route_duration, standby;
    std::string vehicle;
    std::string src;
    std::string dist;
public:
    Edge();

    Edge(int route_duration, std::string vehicle, int standby, std::string src, std::string dist);

    void update_edge(int rdur, std::string vic, int stby);

    int get_duration() const;

    std::string get_vehicle();

    int get_standby() const;

    void set_duration(int dur);

    void set_vehicle(const std::string &vic);

    void set_standby(int stby);

    int getWeight() const;

    std::string getDest() const;

    std::string getSrc() const;


};


#endif //HW02ADV_EDGE_H
