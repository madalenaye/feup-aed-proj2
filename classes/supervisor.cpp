
//
// Created by Madalena Ye on 27/12/2022.
//

#include <climits>
#include "supervisor.h"

Supervisor::Supervisor() {
    createAirports();
    createAirlines();
    createGraph();
    countAirportsPerCountry();
}

unordered_map<string,int> Supervisor::getMap() const{
    return id_airports;
}

Graph Supervisor::getGraph() const {return graph;}

map<string,int> Supervisor::getNrAirportsPerCountry() const {return nrAirportsPerCountry;}

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
        auto d = Graph::distance(airports.find(Airport(source))->getLatitude(),airports.find(Airport(source))->getLongitude()
                ,airports.find(Airport(target))->getLatitude(),airports.find(Airport(target))->getLongitude());
        graph.addEdge(id_airports[source],id_airports[target],Airline(airline),d);
    }
}

bool Supervisor::isCountry(const string& country){
    auto i = countries.find(country);
    if (i == countries.end()) return false;
    return true;
}

bool Supervisor::isAirport(const Airport& airport){
    auto i = airports.find(airport);
    if (i == airports.end()) return false;
    return true;
}

bool Supervisor::isAirline(const Airline& airline) {
    auto i = airlines.find(airline);
    if (i == airlines.end()) return false;
    return true;
}

bool Supervisor::isCity(const string& city) {
    auto i = cities.find(city);
    if (i == cities.end()) return false;
    return true;
}
bool Supervisor::isValidCity(const string& country, const string& city) {
    for (const auto& c : citiesPerCountry[country]){
        if (city == c) return true;
    }
    return false;
}

vector<string> Supervisor::localAirports(double latitude, double longitude, double radius) {
    vector<string> airports;
    for (const auto& node : graph.getNodes()){
        double latitude1 = node.airport.getLatitude();
        double longitude1 = node.airport.getLongitude();
        if (Graph::distance(latitude,longitude,latitude1,longitude1) <= radius)
            airports.push_back(node.airport.getCode());
    }
    return airports;
}

int Supervisor::countAirlinesPerCountry(string country) {
    int count = 0;
    for (auto airline : airlines){
        if (airline.getCountry() == country)
            count++;
    }
    return count;
}

void Supervisor::countAirportsPerCountry() {
    map<string, int> airportsPerCountry;
    for (const auto& i : id_city){
        if (airportsPerCountry.find(i.first.first) == airportsPerCountry.end())
            airportsPerCountry[i.first.first] = i.second.size();
        else{
            auto m = airportsPerCountry.find(i.first.first);
            m->second+= i.second.size();
        }
    }
    nrAirportsPerCountry = airportsPerCountry;
}

template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B,A> flip_map(const std::map<A,B> &src)
{
    std::multimap<B,A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                   flip_pair<A,B>);
    return dst;
}

multimap<int,string> Supervisor::convertMap(const map<string, int>& m) {
    return flip_map(m);
}

int Supervisor::nrFlights(){
    int nrFlights = 0;
    for (const auto& node: graph.getNodes())
        nrFlights += node.adj.size();
    return nrFlights;
}

list<pair<string,string>> Supervisor::processFlight(vector<string> src, vector<string> dest,
                         unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines) {
    int bestFlight = INT_MAX;
    int nrFlights;
    string bestSource, bestTarget;
    list<pair<string,string>> res;
    for (const auto &s: src)
        for (const auto &d: dest) {
            if (src == dest)
                continue;
            nrFlights = graph.nrFlights(id_airports[s], id_airports[d], airlines);
            if (nrFlights != -1 && nrFlights < bestFlight)
                bestFlight = nrFlights;
        }
    for (const auto &s: src)
        for (const auto &d: dest) {
            if (src == dest)
                continue;
            nrFlights = graph.nrFlights(id_airports[s], id_airports[d], airlines);
            if (nrFlights  == bestFlight){
                res.push_back({s,d});
            }
        }
    return res;
}

list<pair<string,string>> Supervisor::processDistance(vector<string> src, vector<string> dest,
                                                    unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines) {
    double bestDistance = 99999999999999;
    double distance;
    string bestSource, bestTarget;
    list<pair<string,string>> res;
    for (const auto &s: src)
        for (const auto &d: dest) {
            if (src == dest)
                continue;
            distance = graph.flownDistance(id_airports[s], id_airports[d], airlines);
            if (distance < bestDistance)
                bestDistance = distance;
        }

    for (const auto &s: src)
        for (const auto &d: dest) {
            if (s == d)
                continue;
            distance = graph.flownDistance(id_airports[s], id_airports[d], airlines);
            if (bestDistance  == distance){
                res.push_back({s,d});
            }
        }
    return res;
}
