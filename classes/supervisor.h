#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <unordered_map>
#include <map>
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

    Airport::AirportH const& getAirports() const;
    Airline::AirlineH const& getAirlines() const;
    Airport::CityH const &getCity() const;
    Graph getGraph() const;
    unordered_map<string,int> getMap() const;
    map<string,int> getNrAirportsPerCountry() const;

    bool isCountry(const string& country);
    bool isAirport(const Airport& airport);
    bool isAirline(const Airline& airline);
    bool isCity(const string& city);
    bool isValidCity(const string& country, const string& city);

    vector<string> localAirports(double, double, double);
    list<pair<string,string>> processFlight(int&, const vector<string>&,const vector<string>&,const Airline::AirlineH&);
    list<pair<string,string>> processDistance(double&, const vector<string>&,const vector<string>&,const Airline::AirlineH&);

    void countAirportsPerCountry();
    int countAirlinesPerCountry(const string& country);
    int nrFlights();
    static multimap<int,string> convertMap(const map<string,int>& m);

private:
    void createAirports();
    void createAirlines();
    void createGraph();

    Airport::AirportH airports;
    Airline::AirlineH airlines;
    Graph graph = Graph(3019);
    unordered_map<string, int> idAirports;
    unordered_set<string> countries;
    unordered_set<string> cities;
    Airport::CityH airportsPerCity;
    unordered_map<string, list<string>> citiesPerCountry;
    map<string, int> nrAirportsPerCountry;
};

#endif //SUPERVISOR_H
