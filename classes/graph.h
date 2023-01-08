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
        list <Edge> adj;
        Airport airport = Airport("");
        bool visited;
        double distance;
        vector<int> parents;
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
    static double distance(double lat1, double lon1, double lat2, double lon2);

    int nrFlights(int src, int dest, Airline::AirlineH airlines);
    int airlineFlights(const string& airline);

    vector<pair<int, string>> flightsPerAirport();
    vector<pair<int, string>> airlinesPerAirport();

    Airport::AirportH listAirports(int nI, int max);
    Airport::CityH2 listCities(int nI, int max);
    set<string> listCountries(int nI, int max);

    struct PairStringHash{
        int operator()(const pair<string,string> &b) const {
            string code = b.second;
            int v = 0;
            for (char i : code)
                v = 37 * v + i;
            return v;
        }
        bool operator()(const pair<string,string> &b1, const pair<string,string> &b2) const {
            return b1.first == b2.first && b1.second== b2.second;
        }
    };
    typedef unordered_set<pair<string, string>, PairStringHash, PairStringHash> PairH;
    PairH airportsFromAirport(int source);
    unordered_set<string> airlinesFromAirport(int i);
    Airport::CityH2 targetsFromAirport(int i);
    unordered_set<string> countriesFromAirport(int i);

    void bfsPath(int src, Airline::AirlineH airlines);
    void findPaths(vector<vector<int>>& paths,vector<int>& path,int v);

    Node dijkstra(int src, int dest, Airline::AirlineH airlines);

    double bfsDiameter(int v);
    double diameter();

    void dfsArt(int v, int index,list<int> &res, Airline::AirlineH airlines);
    list<int> articulationPoints(const Airline::AirlineH& airlines);

    vector<string> getAirlines(int src, int dest, Airline::AirlineH airlines);
    void printPath(vector<int>,const Airline::AirlineH&);
    void printPathsByFlights(int& nrPath, int start, int end, const Airline::AirlineH& airlines);
    void printPathsByDistance(int& nrPath, int start, int end, const Airline::AirlineH& airlines);
};
#endif //GRAPH_H