//
// Created by Madalena Ye on 27/12/2022.
//

#ifndef RENAIR_SUPERVISOR_H
#define RENAIR_SUPERVISOR_H


#include <unordered_set>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include "airport.h"
#include "airline.h"
#include "graph.h"

using namespace std;

class Supervisor {
public:
    Supervisor();
    vector<Airport> getAirports();
    vector<Airline> getAirlines();
private:
    void createAirports();
    void createAirlines();
    vector<Airport> airports;
    vector<Airline> airlines;
    Graph graph = Graph(3019);
    void createGraph();
    //unordered_set<Airport> airports;
    //unordered_set<Airline> airlines;
};

#endif //RENAIR_SUPERVISOR_H
