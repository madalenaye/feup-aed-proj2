#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <list>
#include <queue>
#include <iostream>
#include <cmath>
#include "airport.h"
#include "airline.h"

using namespace std;

class Graph {
    struct Edge {
        int dest;
        Airline airline;
        double distance;
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
    Graph(int nodes);

    void addEdge(int src, int dest, Airline airline, double distance);

    void addAirport(int src, Airport airport);

    int nrFlights(int src, int dest, vector<Airline> airlines);

    double flownDistance(int src, int dest, vector<Airline> airlines);

    list<queue<Airport>> usedAirports(int src, int dest, vector<Airline> airlines);

    list<queue<Airline>> usedAirlines(int src, int dest, vector<Airline> airlines);

    double distance(double lat1, double lon1,double lat2, double lon2);

};

#endif //GRAPH_H