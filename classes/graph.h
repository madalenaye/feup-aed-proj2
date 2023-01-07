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

using namespace std;

class Graph {

    struct Edge {
        int dest{};
        Airline airline;
        double distance{};
    };

    struct Node {
        list <Edge> adj;
        Airport airport = Airport("");
        bool visited;
        double distance;
        int nrFlights;
        queue<Airport> visitedAirports;
        queue<Airline> visitedAirlines;
        int num=0;
        int low;
        bool art;
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

    int nrFlights(int src, int dest, Airline::AirlineH airlines);

    double flownDistance(int src, int dest, Airline::AirlineH airlines);

    list <queue<Airport>> usedAirportsFlights(int src, int dest, Airline::AirlineH airlines);

    list <queue<Airline>> usedAirlinesFlights(int src, int dest, Airline::AirlineH airlines);

    list <queue<Airport>> usedAirportsDistance(int src, int dest, Airline::AirlineH airlines);

    list <queue<Airline>> usedAirlinesDistance(int src, int dest, Airline::AirlineH airlines);

    vector<pair<int, string>> flightsPerAirport();

    vector<pair<int, string>> airlinesPerAirport();

    Airport::AirportH listAirports(int nI, int max);

    Airport::CityH2 listCities(int nI, int max);

    set<string> listCountries(int nI, int max);

    void dfs(int i, const Airline&);
    unordered_set<string> airlinesFromAirport(int i);

    Airport::CityH2 targetsFromAirport(int i);

    unordered_set<string> countriesFromAirport(int i);
    struct PairStringHash{
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
    typedef unordered_set<pair<string, string>, PairStringHash, PairStringHash> PairH;
    PairH AirportsFromAirport(int source);

    list<int> articulationPoints(Airline::AirlineH airlines);
    void dfs_art(int v, int index,list<int> &res, Airline::AirlineH airlines);
    double bfs_diameter(int v);
    double diameter();
};

#endif //GRAPH_H