//
// Created by Madalena Ye on 27/12/2022.
//


#include <queue>
#include "graph.h"

// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int size) : nodes(size+1){
    this->size = size;
}

// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, string airline, double distance) {
    if (src<1 || src>size || dest<1 || dest>size) return;
    nodes[src].adj.push_back({dest,airline,distance});
}

void Graph::addAirport(int src, Airport airport) {
    if (src<1 || src>size) return;
    nodes[src].airport=airport;
}

pair<vector<vector<Airport>>,vector<vector<string>>> Graph::bfs(int src, int dest, vector<string> airlines) {
    for (int i=1; i<=size; i++) {
        nodes[i].visited = false;
        nodes[i].dist = 0;
    }

    vector<vector<Airport>> res;
    vector<vector<string>> res2;
    vector<string> fir;
    queue<int> q; // queue of unvisited nodes
    q.push(src);
    nodes[src].visited = true;
    vector<Airport> init;
    init.push_back(nodes[src].airport);
    nodes[src].visitedAirports = init;
    nodes[src].visitedAirlines = fir;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front(); q.pop();

        // show node order
        // cout << u << " ";
        for (auto e : nodes[u].adj) {
            bool tem = false;
            for (auto x : airlines){
                if (x == e.airline){
                    tem = true;
                    break;
                }
            }
            if (!tem) continue;
            int w = e.dest;
            auto why = e.airline;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].dist = nodes[u].dist + e.distance;

                auto x = nodes[u].visitedAirports;
                x.push_back(nodes[w].airport);
                nodes[w].visitedAirports = x;

                auto y = nodes[u].visitedAirlines;
                y.push_back(e.airline);
                nodes[w].visitedAirlines = y;
            }
            else if (w == dest && nodes[u].visitedAirports.size() == nodes[w].visitedAirports.size()-1){
                auto aux = nodes[u].visitedAirports;
                aux.push_back(nodes[w].airport);
                res.push_back(aux);

                auto aux2 = nodes[u].visitedAirlines;
                aux2.push_back(e.airline);
                res2.push_back(aux2);
            }
        }
    }
    res.push_back(nodes[dest].visitedAirports);
    res2.push_back(nodes[dest].visitedAirlines);
    return {res,res2};
}


int Graph::nrFlights(int src, int dest, vector<string> airlines){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    vector<int> flights(size);
    flights[src] = 0;

    queue<int> q;
    q.push(src);

    nodes[src].visited = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                flights[w] = flights[u] + 1;
            }
        }
    }
    return flights[dest];
}
double Graph::flownDistance(int src, int dest, vector<string> airlines){
    for (int i=1; i<=size; i++){
        nodes[i].visited = false;
        //nodes[i].dist = 0;
    }

    vector<double> distance(size+1);
    distance[src] = 0;

    queue<int> q;
    q.push(src);

    nodes[src].visited = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                distance[w] = distance[u] + e.distance;
                //nodes[w].dist = nodes[u].dist + e.distance;
            }
        }
    }
    //return nodes[dest].dist;
    return distance[dest];
}
list<queue<Airport>> Graph::usedAirports(int src, int dest, vector<string> airlines){
    for (int i=1; i<=size; i++){
        nodes[i].visited = false;
        //nodes[i].dist = 0;
    }
    list<queue<Airport>> res;
    vector<queue<Airport>> airports(size+1);

    airports[src].push(nodes[src].airport);

    queue<int> q;
    q.push(src);

    nodes[src].visited = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                airports[w] = airports[u];
                airports[w].push(nodes[w].airport);
            }
            else if (w == dest && airports[u].size() == airports[w].size()-1){
                queue<Airport> aux = airports[u];
                aux.push(nodes[w].airport);
                res.push_back(aux);
            }
        }
    }
    res.push_back(airports[dest]);
    return res;
}

list<queue<string>> Graph::usedAirlines(int src, int dest, vector<string> airlines){
    for (int i=1; i<=size; i++){
        nodes[i].visited = false;
        nodes[i].dist = 0;
    }
    list<queue<string>> res;
    vector<queue<string>> airline(size+1);


    queue<int> q;
    q.push(src);

    nodes[src].visited = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].dist = nodes[u].dist + 1;
                nodes[w].visited = true;
                airline[w] = airline[u];
                airline[w].push(e.airline);
            }
            else if (w == dest && nodes[u].dist == nodes[w].dist-1){
                queue<string> aux = airline[u];
                aux.push(e.airline);
                res.push_back(aux);
            }
        }
    }
    res.push_back(airline[dest]);
    return res;
}
/*
pair<vector<string>,vector<Airport>> Graph::bfs(int src, int dest, vector<string> airlines) {
    for (int i=1; i<=size; i++) {
        nodes[i].visited = false;
        nodes[i].dist = 0;
    }
    pair<vector<string>,vector<Airport>> res;
    vector<string> fir;
    queue<int> q; // queue of unvisited nodes
    q.push(src);
    nodes[src].visited = true;
    vector<Airport> init;
    init.push_back(nodes[src].airport);
    nodes[src].visitedAirports = init;
    nodes[src].visitedAirlines = fir;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front(); q.pop();

        // show node order
        // cout << u << " ";
        for (auto e : nodes[u].adj) {
            bool tem = false;
            for (auto x : airlines){
                if (x == e.airline){
                    tem = true;
                    break;
                }
            }
            if (!tem) continue;
            int w = e.dest;
            auto why = e.airline;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].dist = nodes[u].dist + e.distance;

                auto x = nodes[u].visitedAirports;
                x.push_back(nodes[w].airport);
                nodes[w].visitedAirports = x;

                auto y = nodes[u].visitedAirlines;
                y.push_back(e.airline);
                nodes[w].visitedAirlines = y;
            }
        }
    }
    res.second = nodes[dest].visitedAirports;
    res.first = nodes[dest].visitedAirlines;
    cout << nodes[dest].dist << '\n';
    return res;
}*/

double Graph::distance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) *
                  M_PI / 180.0;
    double dLon = (lon2 - lon1) *
                  M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}