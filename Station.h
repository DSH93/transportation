#ifndef HW02ADV_STATION_H
#define HW02ADV_STATION_H

#include "Edge.h"
#include <vector>
#include <memory>
#include <string>
#include <set>


class Station {
    std::string station_name;
    int transChangeTime;
    std::vector<std::pair<std::shared_ptr<Station>, std::shared_ptr<Edge>>> links;
    std::vector<std::pair<std::shared_ptr<Station>, std::shared_ptr<Edge>>> transposeGraph;
    static std::set<std::shared_ptr<Edge>> edges;
    std::string station_type;
    int d;
    std::string fatherStr;



public:
    Station(std::string stationName, int ChangeTime, std::string  station_type); // constructor

    Station(const Station& other); //copy constructor

    Station& operator=(const Station& other); //copy assigment operator

    Station(Station&& other) noexcept; // move constructor

    Station& operator=(Station&& other) noexcept; // move assigment operator

    virtual ~Station(); // destructor



    void addStation(const std::shared_ptr<Station>& dest, const std::shared_ptr<Edge>& roth);

    void addStationTransposeGraph(const std::shared_ptr<Station>& father, const std::shared_ptr<Edge>& roth);


    int get_d() const;

    void set_d(int d_value);

    void getTransNeighbors(const std::string& vehicle);

    std::string getStationName() const;

    std::string getStationType() const;

    static bool isVisit(const std::shared_ptr<Edge>& edge);

    void sortStations();

    static void clearEdges();

    std::vector<std::pair<std::shared_ptr<Station>, std::shared_ptr<Edge>>> getLinks() const;

    int getStationChangeTime() const;

    std::string getFatherStr() const;

    void setFatherStr(const std::string& father);


    void getNeighbors(int lastWeight, const std::string &dest, bool isDijkstra, const std::string &src, int currentEdge,
                      const std::string &vehicle, bool isLimited, const std::string & last);
};


#endif //HW02ADV_STATION_H
