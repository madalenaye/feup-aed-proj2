#include "graph.h"
#include "supervisor.h"
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
    for (int i = 1; i <= size; i++) {

        nodes[i].visited = false;
        nodes[i].distance = 0;
    }

    queue<int> q;
    q.push(src);

    nodes[src].visited = true;

    while(!q.empty()){
        int u = q.front(); q.pop();
        for (const Edge& e : nodes[u].adj){
            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].distance = nodes[u].distance + 1;
            }
        }
    }

    return nodes[dest].distance;
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
void Graph::dfsArt(int v, int index, list<int>& res,Airline::AirlineH airlines) {
    nodes[v].num = nodes[v].low = index;
    index = index+1;
    nodes[v].art = true;
    int count = 0;
    for (const auto& e : nodes[v].adj){
        auto w = e.dest;
        if(airlines.find(e.airline) != airlines.end() || airlines.empty())
            if (nodes[w].num == 0){
                count++;
                dfsArt(w,index,res,airlines);
                nodes[v].low = min(nodes[v].low,nodes[w].low);
                if (nodes[w].low >= nodes[v].num && std::find(res.begin(),res.end(),v) == res.end()) {
                    if (index == 2 && count > 1) res.push_back(1);
                    else if (index != 2 && std::find(res.begin(),res.end(),v)== res.end()) res.push_back(v);
                }
            }
            else if (nodes[v].art) {
                nodes[v].low = min(nodes[v].low, nodes[w].num);
            }
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
list<int> Graph::articulationPoints(const Airline::AirlineH& airlines) {
    list<int> answer;

    for (int i = 1; i <= size; i++)
        nodes[i].visited = nodes[i].art = false;

    int index = 1;
    for (int i = 1; i <= size; i++)
        if (nodes[i].num == 0){
            dfsArt(i,index,answer,airlines);
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

Graph::Node Graph::dijkstra(int src, int dest, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines) {

    MinHeap<int, int> q(size, -1);

    for (int v=1; v<=size; v++) {
        nodes[v].distance = INF;
        q.insert(v, INF);
        nodes[v].visited = false;
        nodes[v].parents.clear();
    }

    nodes[src].distance = 0;
    nodes[src].parents.push_back(src);
    q.decreaseKey(src, 0);

    while (q.getSize()>0) {
        int u = q.removeMin();
        nodes[u].visited = true;

        for (auto e : nodes[u].adj) {

            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;

            int v = e.dest;
            double w = e.distance;

            if (!nodes[v].visited && nodes[u].distance + w < nodes[v].distance) {
                nodes[v].distance = nodes[u].distance + w;
                auto aux =nodes[u].parents;
                if (find(aux.begin(),aux.end(),v) == aux.end()) aux.push_back(v);
                nodes[v].parents = aux;
                q.decreaseKey(v, nodes[v].distance);
            }
        }
    }
    return nodes[dest];
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
    for (const Edge& e : nodes[i].adj)
        ans.insert(e.airline.getCode());
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
    for (const auto& e:nodes[i].adj){
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
    for (const Edge& e : nodes[i].adj){
        int w = e.dest;
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
    nodes[nI].distance = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& e : nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].distance = nodes[u].distance +1;
                if (nodes[w].distance<=max) airports.insert(nodes[w].airport);
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
    nodes[nI].distance = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& e : nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].distance = nodes[u].distance +1;
                if (nodes[w].distance<=max) cities.insert({nodes[w].airport.getCountry(),nodes[w].airport.getCity()});
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
set<string> Graph::listCountries(int v, int max) {
    for (int i = 1; i <= size; i++) nodes[i].visited = false;
    std::set<std::string> countries;
    std::queue<int> q;
    q.push(v);
    nodes[v].visited = true;
    nodes[v].distance = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& e : nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].distance = nodes[u].distance +1;
                if (nodes[w].distance<=max) countries.insert(nodes[w].airport.getCountry());
            }
        }
    }
    return countries;
}

vector<string> Graph::getAirlines(int src, int dest,unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines) {
    vector<string> usedAirlines;
    for (auto e: nodes[src].adj)
        if (e.dest == dest && (airlines.empty() ||airlines.find(e.airline) != airlines.end()))
            usedAirlines.push_back(e.airline.getCode());
    return usedAirlines;
}


void Graph::findPaths(vector<vector<int>>& paths,vector<int>& path, int v){

    if (v == -1) {
        if (find(paths.begin(),paths.end(),path) == paths.end())
            paths.push_back(path);
        return;
    }

    for (auto par : nodes[v].parents) {
        path.push_back(v);
        findPaths(paths,path, par);
        path.pop_back();
    }
}

void Graph::bfs(int src, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines){

    for (int i = 1; i <= size; i++) {
        nodes[i].distance = INT_MAX;
        nodes[i].parents.clear();
    }
    queue<int> q;

    q.push(src);
    nodes[src].parents = {-1};
    nodes[src].distance = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto e : nodes[u].adj) {
            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;
            int v = e.dest;
            if (nodes[v].distance > nodes[u].distance + 1) {
                nodes[v].distance = nodes[u].distance + 1;
                q.push(v);
                nodes[v].parents.clear();
                nodes[v].parents.push_back(u);
            }
            else if (nodes[v].distance == nodes[u].distance + 1)
                nodes[v].parents.push_back(u);
        }
    }
}

void Graph::printPathsByFlights(int& nrPath, int start, int end, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines) {
    vector<int> path;
    vector<vector<int> > paths;

    bfs(start,airlines);
    findPaths(paths,path,end);

    Supervisor supervisor;
    auto map = supervisor.getMap();
    for (auto v : paths) {
        reverse(v.begin(), v.end());
        vector<vector<string>> usedAirlines;
        cout << " Trajeto nº" << ++nrPath << ": ";
        for (int i = 0; i < v.size()-1; i++) {
            auto possibleAirlines = getAirlines(v[i],v[i+1],airlines);
            printf("\033[1m\033[46m %s \033[0m", nodes[v[i]].airport.getCode().c_str());
            cout <<" --- (";
            for (int j = 0; j < possibleAirlines.size()-1; j++)
                printf("\033[1m\033[32m %s \033[0m |",possibleAirlines[j].c_str());
            printf("\033[1m\033[32m %s \033[0m",possibleAirlines[possibleAirlines.size()-1].c_str());
            cout << ") --- ";
        }
        printf("\033[1m\033[46m %s \033[0m\n\n", nodes[end].airport.getCode().c_str());
    }
}

void Graph::printPathsByDistance(int& nrPath, int start, int end,
                                 unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines) {
    Node node = dijkstra(start,end,airlines);

    if (node.parents.empty()) {
        cout << " Não existem voos\n\n";
        return;
    }

    cout << " Trajeto nº" << ++nrPath << ": ";
    for (int i = 0; i < node.parents.size()-1; i++){
        auto possibleAirlines = getAirlines(node.parents[i],node.parents[i+1],airlines);
        printf("\033[1m\033[46m %s \033[0m", nodes[node.parents[i]].airport.getCode().c_str());
        cout <<" --- (";
        for (int j = 0; j < possibleAirlines.size()-1; j++)
            printf("\033[1m\033[32m %s \033[0m |",possibleAirlines[j].c_str());
        printf("\033[1m\033[32m %s \033[0m",possibleAirlines[possibleAirlines.size()-1].c_str());
        cout << ") --- ";
    }
    printf("\033[1m\033[46m %s \033[0m\n\n", nodes[end].airport.getCode().c_str());
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
Graph::PairH Graph::airportsFromAirport(int source) {
    Graph::PairH ans;
    for(const auto& e:nodes[source].adj){
        ans.insert({nodes[e.dest].airport.getCode(),nodes[e.dest].airport.getName()});
    }
    return ans;
}

double Graph::bfsDiameter(int v) {
    for (Node& node: nodes){node.visited = false; node.distance = -1.0;}
    queue<int> q;
    q.push(v);
    nodes[v].visited = true;
    nodes[v].distance = 0.0;
    double max = 0;
    while(!q.empty()){
        int u = q.front(); q.pop();
        for (const auto& e: nodes[u].adj){
            int w = e.dest;
            if (!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
                nodes[w].distance = nodes[u].distance + 1;
                if (nodes[w].distance > max) max = nodes[w].distance;
            }
        }
    }
    return max;
}
double Graph::diameter() {
    queue<int> waiting;
    waiting.push(1);
    nodes[1].visited = true;
    double max = bfsDiameter(1);
    for (int i = 1; i <= size; i++){
        if (!nodes[i].visited){
            nodes[i].visited = true;
            double diameter = bfsDiameter(i);
            if (diameter > max) max = diameter;
        }
    }
    return max;
}
