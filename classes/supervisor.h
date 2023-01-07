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
    Airport::AirportH const& getAirports() const {return airports;}
    Airline::AirlineH const& getAirlines() const {return airlines;}
    Airport::CityH const &getCity() const {return id_city;}
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

    int countAirlinesPerCountry(const string& country);
    list<pair<string,string>> processFlight(vector<string>& ,vector<string>& ,Airline::AirlineH&);
    list<pair<string,string>> processDistance(vector<string>&,vector<string>&,Airline::AirlineH&);
    multimap<int,string> convertMap(const map<string,int>& m);


private:
    void createAirports();

    void createAirlines();

    Graph graph = Graph(3019);

    void createGraph();

    Airport::AirportH airports;
    Airline::AirlineH airlines;
    Airport::CityH id_city;
    unordered_map<string, int> id_airports;
    unordered_set<string> countries;
    unordered_set<string> cities;
    unordered_map<string, list<string>> citiesPerCountry;
    map<string, int> nrAirportsPerCountry;
};

#endif //SUPERVISOR_H
