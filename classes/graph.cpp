#include <set>
#include <algorithm>
#include <unordered_set>
#include "graph.h"
#include <climits>
#include <utility>

/**
 * Graph class constructor
 * @param size - number of nodes
 */

Graph::Graph(int size) : nodes(size+1){
    this->size = size;
}
/**
 * Adds an edge from source to destination with an airline and distance\n \n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param src - source node
 * @param dest - target node
 * @param airline - airline
 * @param distance - distance between source airport and target airport
 */
// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, Airline airline, double distance) {
    if (src<1 || src>size || dest<1 || dest>size) return;
    nodes[src].adj.push_back({dest,std::move(airline),distance});
}
/**
 * Adds an airport to a node\n \n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param v - node
 * @param airport - airport
 */
void Graph::addAirport(int src, Airport airport) {
    if (src<1 || src>size) return;
    nodes[src].airport = std::move(airport);
}

vector<Graph::Node> Graph::getNodes() const{
    return nodes;
}
/**
 * Calculates the minimum number of flights between source airport and target airport using airlines \n \n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param src - source node / node of source airport
 * @param dest - target node
 * @param airlines - unordered set of airlines to use (if empty, use all airlines)
 * @return minimum number of flights between source airport and target airport using airlines
 */
int Graph::nrFlights(int src, int dest, unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> airlines){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;

    queue<int> q;
    q.push(src);

    int nrFlights = INT_MAX;
    nodes[src].nrFlights = 0;
    nodes[src].visited = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (const Edge& e : nodes[u].adj){
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
/**
 * Calculates the distance between two points on a sphere using their latitudes and longitudes measured along the surface
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(log(n))</b>
 * </pre>
 * @param lat1 - latitude of point 1
 * @param lon1 - longitude of point 1
 * @param lat2 - latitude of point 2
 * @param lon2 - longitude of point 2
 * @return distance between two points on a sphere
 */
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

pair<int, queue<Airport>> Graph::longestFlight(int src, const Airline& airline){
    for (int i=1; i<=size; i++)
        nodes[i].visited = false;
    pair<int, queue<Airport>> d;
    queue<int> q;
    q.push(src);

    Airport airport("");
    int distance = INT_MIN;
    nodes[src].nrFlights = 0;
    nodes[src].visited = true;
    nodes[src].visitedAirports = queue<Airport>();
    nodes[src].visitedAirports.push(nodes[src].airport);

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (const Edge& e : nodes[u].adj){
            if (e.airline.getCode() != airline.getCode()) continue;
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].nrFlights = nodes[u].nrFlights + 1;
                nodes[w].visitedAirports = nodes[u].visitedAirports;
                nodes[w].visitedAirports.push(nodes[w].airport);
                if (nodes[u].nrFlights+1 > distance) {
                    distance = nodes[u].nrFlights + 1;
                    queue<Airport> aux = nodes[u].visitedAirports;
                    aux.push(nodes[w].airport);
                    d.second = aux;
                    d.first = distance;
                }
            }
        }
    }

    return d;
}

/**
 * Calculates the minimum flown distance between source airport and target airport using airlines \n \n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param src - source node / node of source airport
 * @param dest - target node
 * @param airlines - unordered set of airlines to use (if empty, use all airlines)
 * @return minimum flown distance between source airport and target airport using airlines
 */
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
        for (const Edge& e : nodes[u].adj){
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

/**
 * Calculates used airports of all possible paths between source airport and target airport, using airlines and minimum number of flights \n \n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param src - source node / node of source airport
 * @param dest - target node
 * @param airlines - unordered set of airlines to use (if empty, use all airlines)
 * @return list of queues of used airports between source airport and target airport
 */
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
        for (const Edge& e : nodes[u].adj){
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
/**
 * Calculates used airlines of all possible paths between source airport and target airport, using airlines and minimum number of flights \n \n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param src - source node / node of source airport
 * @param dest - target node
 * @param airlines - unordered set of airlines to use (if empty, use all airlines)
 * @return list of queues of used airlines between source airport and target airport
 */
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
        for (const Edge& e : nodes[u].adj){
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
/**
 * Calculates used airports of all possible paths between source airport and target airport, using airlines and minimum flown distance \n \n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param src - source node / node of source airport
 * @param dest - target node
 * @param airlines - unordered set of airlines to use (if empty, use all airlines)
 * @return list of queues of used airports between source airport and target airport
 */
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
        for (const Edge& e : nodes[u].adj){
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
/**
 * Calculates used airlines of all possible paths between source airport and target airport, using airlines and minimum flown distance \n \n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param src - source node / node of source airport
 * @param dest - target node
 * @param airlines - unordered set of airlines to use (if empty, use all airlines)
 * @return list of queues of used airlines between source airport and target airport
 */
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
        for (const Edge& e : nodes[u].adj){
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


bool cmp( const pair<int,string>& a, const pair<int,string>& b){
    return a.first > b.first;
}
/**
 *
 * @return
 */
vector<pair<int, string>> Graph::flightsPerAirport() {
    vector<pair<int,string>> n;
    for (int i = 1; i <= size; i++){
        int nrFlights = nodes[i].adj.size();
        n.emplace_back(nrFlights, nodes[i].airport.getCode());
    }
    sort(n.begin(), n.end(), cmp);
    return n;
}
vector<pair<int,string>> Graph::airlinesPerAirport() {
    vector<pair<int,string>> nrAirlines;
    for (int i = 1; i <= size; i++){
        set<string> n;
        for (const Edge& e : nodes[i].adj){
            n.insert(e.airline.getCode());
        }
        nrAirlines.emplace_back(n.size(), nodes[i].airport.getCode());
    }
    sort(nrAirlines.begin(), nrAirlines.end(), cmp);
    return nrAirlines;
}

unordered_set<string> Graph::airlinesFromAirport(int i) {
    unordered_set<string> ans;
    for(auto e:nodes[i].adj){
        ans.insert(e.airline.getCode());
    }
    return ans;
}
unordered_set<pair<string, string>, Airport::CityHash, Airport::CityHash> Graph::targetsFromAirport(int i){
    unordered_set<pair<string,string>, Airport::CityHash, Airport::CityHash> ans;
    for (auto e:nodes[i].adj){
        int w = e.dest;
        ans.insert({nodes[w].airport.getCountry(),nodes[w].airport.getCity()});
    }
    return ans;
}
unordered_set<string> Graph::countriesFromAirport(int i) {
    unordered_set<string> ans;
    for(auto e:nodes[i].adj){
        int w=e.dest;
        ans.insert(nodes[w].airport.getCountry());
    }
    return ans;
}
/**
 * Calculates the reachable airports using "max" number of flights
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param v - source node
 * @param max - number of flights
 * @return set of reachable airports using "max" number of flights
 */
unordered_set<Airport, Airport::AirportHash, Airport::AirportHash> Graph::listAirports(int nI, int max) {
    for (Node& node: nodes) node.visited = false;
    unordered_set<Airport,Airport::AirportHash, Airport::AirportHash> airports;
    std::queue<int> q; // queue of unvisited nodes
    q.push(nI);
    nodes[nI].visited = true;
    nodes[nI].nrFlights = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& e : nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].nrFlights = nodes[u].nrFlights +1;
                if (nodes[w].nrFlights<=max) airports.insert(nodes[w].airport);
            }
        }
    }
    return airports;
}
/**
 * Calculates the reachable cities using "max" number of flights
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param v - source node
 * @param max - number of flights
 * @return unordered_set of reachable cities using "max" number of flights
 */
unordered_set<pair<string,string>, Airport::CityHash, Airport::CityHash> Graph::listCities(int nI, int max) {
    for (Node& node: nodes) node.visited = false;
    unordered_set<pair<string,string>, Airport::CityHash, Airport::CityHash> cities;
    std::queue<int> q; // queue of unvisited nodes
    q.push(nI);
    nodes[nI].visited = true;
    nodes[nI].nrFlights = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& e : nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].nrFlights = nodes[u].nrFlights +1;
                if (nodes[w].nrFlights<=max) cities.insert({nodes[w].airport.getCountry(),nodes[w].airport.getCity()});
            }
        }
    }
    return cities;
}
/**
 * Calculates the reachable countries using "max" number of flights
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param v - source node
 * @param max - number of flights
 * @return set of reachable countries using "max" number of flights
 */
set<string> Graph::listCountries(int nI, int max) {
    for (Node& node: nodes) node.visited = false;
    std::set<std::string> countries;
    std::queue<int> q; // queue of unvisited nodes
    q.push(nI);
    nodes[nI].visited = true;
    nodes[nI].nrFlights = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& e : nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].nrFlights = nodes[u].nrFlights +1;
                if (nodes[w].nrFlights<=max) countries.insert(nodes[w].airport.getCountry());
            }
        }
    }
    return countries;
}