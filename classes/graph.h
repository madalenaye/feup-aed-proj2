//
// Created by Madalena Ye on 27/12/2022.
//

#ifndef RENAIR_GRAPH_H
#define RENAIR_GRAPH_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "airport.h"
#include "airline.h"

using namespace std;

class Graph {
    struct Edge {
        int dest;   // Destination node
        string airline;
        double distance; // An integer weight
    };

    struct Node {
        list<Edge> adj; // The list of outgoing edges (to adjacent nodes)
        bool visited; // As the node been visited on a search?
        Airport airport = Airport("XXX","XXX","XXX","XXX",0,0);
        int dist;
        vector<Airport> visitedAirports;
        vector<Airline> visitedAirlines;
    };

    int size;              // Graph size (vertices are numbered from 1 to n)
    vector<Node> nodes; // The list of nodes being represented

public:
    // Constructor: nr nodes and direction (default: directed)
    Graph(int nodes);
    Graph();

    // Add edge from source to destination with a certain weight
    void addEdge(int src, int dest, string airline);

    void addAirport(int src, Airport airport);

    // Depth-First Search: example implementation
    //void dfs(string v);
    void bfs(int v);
    pair<vector<string>,vector<Airport>> distance(int src, int dest, vector<string> airlines);

};

#endif //RENAIR_GRAPH_H
