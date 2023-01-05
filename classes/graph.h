#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_set>
#include <list>
#include <queue>
#include <iostream>
#include <cmath>
#include "airport.h"
#include "airline.h"

using namespace std;

class Graph {

    struct Edge {
        int dest{};
        Airline airline;
        double distance{};
    };

    struct Node {
        list<Edge> adj;
        Airport airport = Airport("");
        bool visited;
        double distance;
        int nrFlights;
        queue<Airport> visitedAirports;
        queue<Airline> visitedAirlines;
    };

    int size;
    vector<Node> nodes;

public:

    explicit Graph(int nodes);

    void addEdge(int src, int dest, Airline airline, double distance);

    void addAirport(int src, Airport airport);

    vector<Node> getNodes() const;

    pair<int, queue<Airport>> diameter(int src, const Airline& airline);
    static double distance(double lat1, double lon1,double lat2, double lon2);

    int nrFlights(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines);
    double flownDistance(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines);

    list<queue<Airport>> usedAirportsFlights(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines);
    list<queue<Airline>> usedAirlinesFlights(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines);
    list<queue<Airport>> usedAirportsDistance(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines);
    list<queue<Airline>> usedAirlinesDistance(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines);

    vector<pair<int,string>> flightsPerAirport();
    vector<pair<int,string>> airlinesPerAirport();

    int countCountries(int nI, int max);
    int countAirports(int nI, int max);
    int countCities(int nI, int max);
};

#endif //GRAPH_H