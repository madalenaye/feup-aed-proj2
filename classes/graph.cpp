#include "graph.h"

// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int size) : nodes(size+1){
    this->size = size+1;
}

// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, Airline airline, double distance) {
    if (src<1 || src>size || dest<1 || dest>size) return;
    nodes[src].adj.push_back({dest,airline,distance});
}

void Graph::addAirport(int src, Airport airport) {
    if (src<1 || src>size) return;
    nodes[src].airport = airport;
}

int Graph::nrFlights(int src, int dest, vector<Airline> airlines){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    queue<int> q;
    q.push(src);

    nodes[src].nrFlights = 0;
    nodes[src].visited = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].nrFlights = nodes[u].nrFlights + 1;
            }
        }
    }
    return nodes[dest].nrFlights;
}

double Graph::flownDistance(int src, int dest, vector<Airline> airlines){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    queue<int> q;
    q.push(src);

    nodes[src].visited = true;
    nodes[src].distance = 0;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].distance = nodes[u].distance + e.distance;
            }
        }
    }
    return nodes[dest].distance;
}

list<queue<Airport>> Graph::usedAirports(int src, int dest, vector<Airline> airlines){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    list<queue<Airport>> res;

    queue<int> q;
    q.push(src);

    nodes[src].visited = true;
    nodes[src].visitedAirports.push(nodes[src].airport);

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].visitedAirports = nodes[u].visitedAirports;
                nodes[w].visitedAirports.push(nodes[w].airport);
            }
            else if (w == dest && nodes[u].visitedAirports.size() == nodes[w].visitedAirports.size()-1){
                queue<Airport> aux = nodes[u].visitedAirports;
                aux.push(nodes[w].airport);
                res.push_back(aux);
            }
        }
    }
    res.push_back(nodes[dest].visitedAirports);
    return res;
}

list<queue<Airline>> Graph::usedAirlines(int src, int dest, vector<Airline> airlines){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    list<queue<Airline>> res;

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
                nodes[w].visitedAirlines = nodes[u].visitedAirlines;
                nodes[w].visitedAirlines.push(e.airline);
            }
            else if (w == dest && nodes[u].visitedAirlines.size() == nodes[w].visitedAirlines.size()-1){
                queue<Airline> aux = nodes[u].visitedAirlines;
                aux.push(e.airline);
                res.push_back(aux);
            }
        }
    }
    res.push_back(nodes[dest].visitedAirlines);
    return res;
}

double Graph::distance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}