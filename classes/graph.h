#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_set>
#include <list>
#include <queue>
#include <iostream>
#include <stack>
#include <cmath>
#include <set>
#include <algorithm>
#include <climits>
#include <utility>
#include "airport.h"
#include "airline.h"
#include "minHeap.h"

#define INF (INT_MAX/2)

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
        vector<int> parents;
    };

    int size;
    vector<Node> nodes;

public:

    explicit Graph(int nodes);

    void addEdge(int src, int dest, Airline airline, double distance);

    void addAirport(int src, Airport airport);

    vector<Node> getNodes() const;

    stack<Airport> longestFlight(const Airline &airline);

    static double distance(double lat1, double lon1, double lat2, double lon2);

    int nrFlights(int src, int dest, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);

    double flownDistance(int src, int dest, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);
    vector<string> getAirlines(int src, int dest, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);
    list<queue<Airport>>
    usedAirportsFlights(int src, int dest, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);

    list<queue<Airline>>
    usedAirlinesFlights(int src, int dest, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);

    list<queue<Airport>> usedAirportsDistance(int src, int dest,
                                              unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);

    list<queue<Airline>> usedAirlinesDistance(int src, int dest,
                                              unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);

    vector<pair<int, string>> flightsPerAirport();

    vector<pair<int, string>> airlinesPerAirport();

    unordered_set<Airport, Airport::AirportHash, Airport::AirportHash> listAirports(int nI, int max);

    unordered_set<pair<string, string>, Airport::CityHash, Airport::CityHash> listCities(int nI, int max);

    set<string> listCountries(int nI, int max);

    void dfs(int i, const Airline &);

    unordered_set<string> airlinesFromAirport(int i);

    unordered_set<pair<string, string>, Airport::CityHash, Airport::CityHash> targetsFromAirport(int i);

    unordered_set<string> countriesFromAirport(int i);
    Node dijkstra(int src, int dest, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);

    void bfs(int src, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);
    void findPaths(vector<vector<int>>& paths,vector<int>& path,int v);
    void printPathsByFlights(int& nrPath, int start, int end, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);
    void printPathsByDistance(int& nrPath, int start, int end, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines);

};
#endif //GRAPH_H