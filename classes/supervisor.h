//
// Created by Madalena Ye on 27/12/2022.
//

#ifndef RENAIR_SUPERVISOR_H
#define RENAIR_SUPERVISOR_H


#include <unordered_set>
#include <unordered_map>
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
    struct AirlineHash {
        // TODO
        int operator()(const Airline &b) const {
            string code = b.getCode();
            int v = 0;
            for (unsigned int i = 0; i < code.size(); i++)
                v = 37 * v + code[i];
            return v;
        }

        // TODO
        bool operator()(const Airline &b1, const Airline &b2) const {
            return b1.getCode() == b2.getCode();
        }
    };

    struct AirportHash {
        // TODO
        int operator()(const Airport &b) const {
            string code = b.getCode();
            int v = 0;
            for (unsigned int i = 0; i < code.size(); i++)
                v = 37 * v + code[i];
            return v;
        }

        // TODO
        bool operator()(const Airport &b1, const Airport &b2) const {
            return b1.getCode() == b2.getCode();
        }
    };
    unordered_set<Airport,AirportHash,AirportHash> const& getAirports() const {return airports;}
    unordered_set<Airline,AirlineHash,AirlineHash> const& getAirlines() const {return airlines;}
    Graph getGraph() const;
private:
    void createAirports();

    void createAirlines();

    Graph graph = Graph(3019);

    void createGraph();

    unordered_set<Airport, AirportHash,AirportHash> airports;
    unordered_set<Airline,AirlineHash,AirlineHash> airlines;
    unordered_map<string, int> id_airports;


};
#endif //RENAIR_SUPERVISOR_H
