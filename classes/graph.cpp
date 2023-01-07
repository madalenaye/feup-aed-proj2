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
int Graph::nrFlights(int src, int dest, Airline::AirlineH airlines){
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

stack<Airport> Graph::longestFlight(const Airline& airline){

    for (int i = 1; i <= size; i++) {
        nodes[i].nrFlights = 0;
        nodes[i].visited = false;
    }

    for (int i = 1; i <= size; i++)
        if (!nodes[i].visited)
            dfs(i, airline);

    int ans = 0;
    stack<Airport> used;

    for (int i = 1; i <= size; i++)
        if (nodes[i].nrFlights > ans) {
            ans = nodes[i].nrFlights;
            used = stack<Airport>();
            while (!nodes[i].visitedAirports.empty()) {
                used.push(nodes[i].visitedAirports.front());
                nodes[i].visitedAirports.pop();
            }
        }

    return used;
}
/**
 * Calculates the biggest distance from a node to it's child
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V|+|E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param v -> source node
 * @param airline -> wanted airline
 */

void Graph::dfs(int v, const Airline airline){
    nodes[v].visited = true ;
    //auto aux = nodes[v].visitedAirports;
    //aux.push(nodes[v].airport);
    for ( auto e : nodes[v].adj) {
        Airline a = e.airline;
        if (a.getCode() != airline.getCode()) continue;

        int w = e.dest;
        if (!nodes[w].visited) {
            dfs(w, airline);
        }
        if (1+nodes[w].nrFlights > nodes[v].nrFlights) {
            auto aux = nodes[w].visitedAirports;
            aux.push(nodes[v].airport);
            nodes[v].visitedAirports = aux;
            nodes[v].nrFlights = 1 + nodes[w].nrFlights;
        }
    }
    if (nodes[v].nrFlights == 0)
        nodes[v].visitedAirports.push(nodes[v].airport);
}
/**
 * Finds the nodes that are articulation points and inserts them in a list\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O((|E|)*n)</b>,E -> number of edges, n-> list size
 * </pre>
 * @param v -> source node
 * @param index
 * @param res -> list of articulation points
 * @param airlines -> unordered_set of Airlines that can be empty(meaning it searches all airlines) or with some specific user input airlines
 */
void Graph::dfs_art(int v, int index, list<int>& res,Airline::AirlineH airlines) {
    nodes[v].num = nodes[v].low = index;
    index = index+1;
    nodes[v].art = true;
    int count = 0;
    for (auto e : nodes[v].adj){
        auto w = e.dest;
        if(airlines.find(e.airline) != airlines.end() || airlines.empty())
        if (nodes[w].num == 0){
            count++;
            dfs_art(w,index,res,airlines);
            nodes[v].low = min(nodes[v].low,nodes[w].low);
            if (nodes[w].low >= nodes[v].num && std::find(res.begin(),res.end(),v) == res.end()) {
                if (index == 2 && count > 1) res.push_back(1);
                else if (index != 2 && std::find(res.begin(),res.end(),v)== res.end()) res.push_back(v);
            }
        }
        else if (nodes[v].art)
            nodes[v].low = min(nodes[v].low,nodes[w].num);
    }
}
/**
 * Calculates the list of articulation points that exist in a specific unordered_set of airlines or in all airlines.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O((|V|+|E|)*n)</b>, V -> number of nodes, E -> number of edges, n-> list size
 * </pre>
 * @param airlines -> unordered_set of Airlines that can be empty(meaning it searches all airlines) or with some specific user input airlines
 * @return The list of articulation points.
 */
list<int> Graph::articulationPoints(Airline::AirlineH airlines) {
    list<int> answer;

    for (int i = 1; i <= size; i++)
        nodes[i].visited = nodes[i].art = false;

    int index = 1;
    for (int i = 1; i <= size; i++)
        if (nodes[i].num == 0){
            dfs_art(i,index,answer,airlines);
        }
    return answer;
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
double Graph::flownDistance(int src, int dest, Airline::AirlineH airlines){
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
list<queue<Airport>> Graph::usedAirportsFlights(int src, int dest, Airline::AirlineH airlines){
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
list<queue<Airline>> Graph::usedAirlinesFlights(int src, int dest, Airline::AirlineH airlines){
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
list<queue<Airport>> Graph::usedAirportsDistance(int src, int dest, Airline::AirlineH airlines){

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
list<queue<Airline>> Graph::usedAirlinesDistance(int src, int dest, Airline::AirlineH airlines){

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
 * Calculates the number of departures of each airport.
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V|)</b>, V -> number of nodes
 * </pre>
 * @return ordered vector of pair<Number of departures,Airport Code> by descending order of number of flights
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
/**
 * Calculates the number of airlines that work with each airport.
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V|)</b>, V -> number of nodes
 * </pre>
 * @return ordered vector of pair<Number of airlines,Airport Code> by descending order of number of airlines
 */
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
/**
 * Calculates the different airlines that cooperate with an airport
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|E|)</b>, E -> number of edges
 * </pre>
 * @param i -> node of source
 * @return set of all the different airlines
 */
unordered_set<string> Graph::airlinesFromAirport(int i) {
    unordered_set<string> ans;
    for(auto e:nodes[i].adj){
        ans.insert(e.airline.getCode());
    }
    return ans;
}
/**
 * Calculates the different cities that are reachable from an airport within 1 flight\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|E|)</b>, E -> number of edges
 * </pre>
 * @param i -> node of source
 * @return set of all the different cities
 */
Airport::CityH2 Graph::targetsFromAirport(int i){
    Airport::CityH2 ans;
    for (auto e:nodes[i].adj){
        int w = e.dest;
        ans.insert({nodes[w].airport.getCountry(),nodes[w].airport.getCity()});
    }
    return ans;
}
/**
 * Calculates the different countries that are reachable from an airport within 1 flight\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|E|)</b>, E -> number of edges
 * </pre>
 * @param i -> node of source
 * @return set of all the different countries
 */
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
Airport::AirportH Graph::listAirports(int nI, int max) {
    for (Node& node: nodes) node.visited = false;
    Airport::AirportH airports;
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
Airport::CityH2 Graph::listCities(int nI, int max) {
    for (Node& node: nodes) node.visited = false;
    Airport::CityH2 cities;
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
/**
 * From a specific airport, calculates all of the airports that are reachable within 1 flight.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|E|)</b>,E -> number of edges
 * </pre>
 * @param source -> source node
 * @return unordered_set of airports code and name
 */
Graph::PairH Graph::AirportsFromAirport(int source) {
    Graph::PairH ans;
    for(auto e:nodes[source].adj){
        ans.insert({nodes[e.dest].airport.getCode(),nodes[e.dest].airport.getName()});
    }
    return ans;
}
