#include <set>
#include <algorithm>
#include <unordered_set>
#include "graph.h"
#include <climits>

// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int size) : nodes(size+1){
    this->size = size;
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

int Graph::nrFlights(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    queue<int> q;
    q.push(src);

    int nrFlights = INTMAX_MAX;
    nodes[src].nrFlights = 0;
    nodes[src].visited = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].nrFlights = nodes[u].nrFlights + 1;
                if (w == dest) nrFlights = nodes[w].nrFlights;
            }
        }
    }

    return nrFlights;
}


pair<int,string> Graph::diameter(int src, Airline airline){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    queue<int> q;
    q.push(src);

    Airport airport("");
    int diameter = INTMAX_MIN;
    nodes[src].nrFlights = 0;
    nodes[src].visited = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            if (e.airline.getCode() != airline.getCode()) continue;
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].nrFlights = nodes[u].nrFlights + 1;
            }
            if (nodes[u].nrFlights+1 > diameter) {
                diameter = nodes[u].nrFlights + 1;
                airport = nodes[w].airport;
            }
        }
    }
    return {diameter,airport.getCode()};
}

double Graph::flownDistance(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    double minDistance = MAXFLOAT;
    queue<int> q;
    q.push(src);

    nodes[src].visited = true;
    nodes[src].distance = 0;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;

            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].distance = nodes[u].distance + e.distance;
            }
            if (w == dest && nodes[u].distance + e.distance < minDistance)
                minDistance = nodes[u].distance + e.distance;
        }
    }
    return minDistance;
}

list<queue<Airport>> Graph::usedAirportsFlights(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    list<queue<Airport>> res;

    queue<int> q;
    q.push(src);

    nodes[src].visited = true;
    nodes[src].visitedAirports = queue<Airport>();
    nodes[src].visitedAirports.push(nodes[src].airport);

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].visitedAirports = nodes[u].visitedAirports;
                nodes[w].visitedAirports.push(nodes[w].airport);
            }
            if (w == dest && nodes[u].visitedAirports.size() == nodes[w].visitedAirports.size()-1){
                queue<Airport> aux = nodes[u].visitedAirports;
                aux.push(nodes[w].airport);
                res.push_back(aux);
            }
        }
    }
    return res;
}

list<queue<Airline>> Graph::usedAirlinesFlights(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    list<queue<Airline>> res;

    queue<int> q;
    q.push(src);


    nodes[src].visited = true;
    nodes[src].visitedAirlines = queue<Airline>();

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;
            int w = e.dest;
            if (!nodes[w].visited){

                q.push(w);
                nodes[w].visited = true;
                nodes[w].visitedAirlines = nodes[u].visitedAirlines;
                nodes[w].visitedAirlines.push(e.airline);
            }
            if (w == dest && nodes[u].visitedAirlines.size() == nodes[w].visitedAirlines.size()-1){
                queue<Airline> aux = nodes[u].visitedAirlines;
                aux.push(e.airline);
                res.push_back(aux);
            }
        }
    }
    return res;
}


list<queue<Airport>> Graph::usedAirportsDistance(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines){

    double distance = flownDistance(src,dest,airlines);

    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    list<queue<Airport>> res;

    queue<int> q;
    q.push(src);

    nodes[src].visited = true;
    nodes[src].distance = 0;
    nodes[src].visitedAirports = queue<Airport>();
    nodes[src].visitedAirports.push(nodes[src].airport);


    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].visitedAirports = nodes[u].visitedAirports;
                nodes[w].visitedAirports.push(nodes[w].airport);
                nodes[w].distance = nodes[u].distance + e.distance;
            }
            if (w == dest && nodes[u].distance + e.distance == distance){
                queue<Airport> aux = nodes[u].visitedAirports;
                aux.push(nodes[w].airport);
                res.push_back(aux);
            }
        }
    }
    return res;
}

list<queue<Airline>> Graph::usedAirlinesDistance(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines){

    double distance = flownDistance(src,dest,airlines);

    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    list<queue<Airline>> res;

    queue<int> q;
    q.push(src);


    nodes[src].visited = true;
    nodes[src].distance = 0;
    nodes[src].visitedAirlines = queue<Airline>();

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (Edge e : nodes[u].adj){
            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;
            int w = e.dest;
            if (!nodes[w].visited){

                q.push(w);
                nodes[w].visited = true;
                nodes[w].visitedAirlines = nodes[u].visitedAirlines;
                nodes[w].visitedAirlines.push(e.airline);
                nodes[w].distance = nodes[u].distance + e.distance;
            }
            if (w == dest && nodes[u].distance + e.distance == distance){
                queue<Airline> aux = nodes[u].visitedAirlines;
                aux.push(e.airline);
                res.push_back(aux);
            }
        }
    }
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

vector<Graph::Node> Graph::getNodes() const{
    return nodes;
}

bool cmp( pair<int,string> a, pair<int,string> b){
    return a.first > b.first;
}

vector<pair<int, string>> Graph::flightsPerAirport() {
    vector<pair<int,string>> n;
    for (int i = 1; i <= size; i++){
        int nrFlights = nodes[i].adj.size();
        n.push_back({nrFlights, nodes[i].airport.getCode()});
    }
    sort(n.begin(), n.end(), cmp);
    return n;
}
vector<pair<int,string>> Graph::airlinesPerAirport() {
    vector<pair<int,string>> nrAirlines;
    for (int i = 1; i <= size; i++){
        set<string> n;
        for (Edge e : nodes[i].adj){
            n.insert(e.airline.getCode());
        }
        nrAirlines.push_back({n.size(), nodes[i].airport.getCode()});
    }
    sort(nrAirlines.begin(), nrAirlines.end(), cmp);
    return nrAirlines;
}