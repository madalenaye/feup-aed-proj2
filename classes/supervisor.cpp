//
// Created by Madalena Ye on 27/12/2022.
//

#include "supervisor.h"

Supervisor::Supervisor() {
    createAirports();
    createAirlines();
    createGraph();
}

unordered_map<string,int> Supervisor::getMap() const{
    return id_airports;
}

Graph Supervisor::getGraph() const {return graph;}

void Supervisor::createAirports() {
    ifstream myFile;
    string currentLine, code, name, city, country, x;
    double latitude, longitude;
    int i = 1;
    myFile.open("../data/airports.csv");
    getline(myFile, currentLine);

    while (getline(myFile,currentLine)){
        istringstream iss(currentLine);

        getline(iss,code,',');
        getline(iss,name,',');
        getline(iss,city,',');
        getline(iss,country,',');
        getline(iss,x,','); latitude = stod(x);
        getline(iss,x,','); longitude = stod(x);

        Airport airport = Airport(code,name,city,country,latitude,longitude);
        graph.addAirport(i,airport);
        id_city[{airport.getCountry(),airport.getCity()}].push_back(code);
        id_airports.insert({airport.getCode(),i++});
        airports.insert(airport);
        countries.insert(country);
        cities.insert(city);
        citiesPerCountry[country].push_back(city);
    }
}

void Supervisor::createAirlines() {
    ifstream inFile;
    string code, name, callsign, country, line;
    inFile.open("../data/airlines.csv");
    getline(inFile, line);
    while(getline(inFile, line)){
        istringstream is(line);
        getline(is,code,',');
        getline(is,name,',');
        getline(is,callsign,',');
        getline(is,country,',');
        Airline a = Airline(code, name, callsign, country);
        airlines.insert(a);
    }
}

void Supervisor::createGraph(){
    ifstream inFile;
    string source, target, airline, line;
    inFile.open("../data/flights.csv");
    getline(inFile, line);
    while(getline(inFile, line)){
        istringstream is(line);
        getline(is,source,',');
        getline(is,target,',');
        getline(is,airline,',');
        auto d = graph.distance(airports.find(Airport(source))->getLatitude(),airports.find(Airport(source))->getLongitude()
                ,airports.find(Airport(target))->getLatitude(),airports.find(Airport(target))->getLongitude());
        graph.addEdge(id_airports[source],id_airports[target],airline,d);
    }
}

bool Supervisor::isCountry(string country){
    auto i = countries.find(country);
    if (i == countries.end()) return false;
    return true;
}

bool Supervisor::isAirport(Airport airport){
    auto i = airports.find(airport);
    if (i == airports.end()) return false;
    return true;
}

bool Supervisor::isAirline(Airline airline) {
    auto i = airlines.find(airline);
    if (i == airlines.end()) return false;
    return true;
}

bool Supervisor::isCity(string city) {
    auto i = cities.find(city);
    if (i == cities.end()) return false;
    return true;
}
bool Supervisor::isValidCity(string country, string city) {
    for (auto c : citiesPerCountry[country]){
        if (city == c) return true;
    }
    return false;
}

vector<string> Supervisor::localAiports(double latitude, double longitude, double radius) {
    vector<string> airports;

    for (auto node : graph.getNodes()){
        double latitude1 = node.airport.getLatitude();
        double longitude1 = node.airport.getLongitude();
        if (graph.distance(latitude,longitude,latitude1,longitude1) <= radius)
            airports.push_back(node.airport.getCode());
    }
    return airports;
}