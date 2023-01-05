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
            for (char i : code)
                v = 37 * v + i;
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

    bool isCountry(const string& country);
    bool isAirport(const Airport& airport);
    bool isAirline(const Airline& airline);
    bool isCity(const string& city);
    bool isValidCity(const string& country, const string& city);

    vector<string> localAirports(double, double, double);
    void countAirportsPerCountry();
    int nrFlights();
    int countAirlinesPerCountry(string country);
    list<pair<string,string>> processFlight(vector<string>,vector<string>,unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash>);
    list<pair<string,string>> processDistance(vector<string>,vector<string>,unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash>);
    multimap<int,string> convertMap(const map<string,int>& m);

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
