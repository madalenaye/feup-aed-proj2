//
// Created by Madalena Ye on 27/12/2022.
//

#ifndef RENAIR_SUPERVISOR_H
#define RENAIR_SUPERVISOR_H


#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include <set>
#include <algorithm>
#include "airport.h"
#include "airline.h"
#include "graph.h"

using namespace std;

class Supervisor {
public:
    Supervisor();
    struct CityHash{
        int operator()(const pair<string,string> &b) const {
            string code = b.second;
            int v = 0;
            for (unsigned int i = 0; i < code.size(); i++)
                v = 37 * v + code[i];
            return v;
        }

        // TODO
        bool operator()(const pair<string,string> &b1, const pair<string,string> &b2) const {
            return b1.first == b2.first && b1.second== b2.second;
        }
    };

    unordered_set<Airport,Airport::AirportHash,Airport::AirportHash> const& getAirports() const {return airports;}
    unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> const& getAirlines() const {return airlines;}
    unordered_map<pair<string,string>, vector<string>,CityHash,CityHash> const &getCity() const {return id_city;}
    Graph getGraph() const;
    unordered_map<string,int> getMap() const;
    map<string,int> getNrAirportsPerCountry() const;
    set<string> getCountries(){return countries;}
    bool isCountry(string country);
    bool isAirport(Airport airport);
    bool isAirline(Airline airline);
    bool isCity(string city);
    bool isValidCity(string country, string city);
    vector<string> localAiports(double, double, double);
    void countAirportsPerCountry();
    multimap<int,string> convertMap(map<string,int> m);
    int nrFlights();

private:
    void createAirports();

    void createAirlines();

    Graph graph = Graph(3019);

    void createGraph();

    unordered_set<Airport, Airport::AirportHash,Airport::AirportHash> airports;
    unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines;
    unordered_map<string, int> id_airports;
    unordered_map<pair<string,string> ,vector<string>,CityHash,CityHash> id_city;
    set<string> countries;
    set<string> cities;
    unordered_map<string, list<string>> citiesPerCountry;
    map<string, int> nrAirportsPerCountry;
};

#endif //RENAIR_SUPERVISOR_H
