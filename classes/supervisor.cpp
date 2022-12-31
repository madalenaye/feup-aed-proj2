//
// Created by Madalena Ye on 27/12/2022.
//

#include "supervisor.h"

Supervisor::Supervisor() {
    createAirports();
    createAirlines();
    createFlights();
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
        id_airports.insert({airport.getCode(),i++});
        airports.insert(airport);
        countries.insert(country);
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
void Supervisor::createFlights() {
    ifstream inFile;
    string source, target, airline, line;
    inFile.open("../data/flights.csv");
    getline(inFile, line);
    while(getline(inFile, line)){
        istringstream is(line);
        getline(is,source,',');
        getline(is,target,',');
        getline(is,airline,',');
        flights.push_back(Flight(source, target, airline));
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

unsigned Supervisor::countFlights(string airport, int flag) {
    int count = 0;
    if (flag == 0){
        for (auto i : flights){
            if (i.getSource() == airport) count++;
        }
    }
    else if (flag == 1){
        unordered_set<string> diffAirlines;
        for (auto i : flights){
            diffAirlines.insert(i.getAirline());
        }
        count = diffAirlines.size();
    }
    else{
        unordered_set<string> diffDestination;
        for (auto i:flights){
            diffDestination.insert(i.getTarget());
        }
        count = diffDestination.size();
    }
    return count;
}
bool Supervisor::isCountry(string country){
    auto i = find(countries.begin(), countries.end(), country);
    if (i == countries.end()) return false;
    return true;
}