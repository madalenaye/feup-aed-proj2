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
    for (int i = 1; i <= size; i++)
        nodes[i].visited = false;

    queue<int> q;
    q.push(src);

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
            }
        }
    }

    return nodes[dest].nrFlights;
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


void Graph::dfs(int v, const Airline& airline){
    nodes[v].visited = true;

    for (const Edge& e : nodes[v].adj) {
        Airline a = e.airline;
        if (a.getCode() != airline.getCode()) continue;

        int w = e.dest;
        if (!nodes[w].visited)
            dfs(w, airline);

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

Graph::Node Graph::dijkstra(int src, int dest, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines) {
    MinHeap<int, int> q(size, -1);
    for (int v=1; v<=size; v++) {
        nodes[v].distance = INF;
        q.insert(v, INF);
        nodes[v].visited = false;
    }
    nodes[src].distance = 0;
    nodes[src].visitedAirports.push(nodes[src].airport);
    q.decreaseKey(src, 0);
    while (q.getSize()>0) {
        int u = q.removeMin();
        //cout << "Node " << u << " with dist = " << nodes[u].dist << endl;
        nodes[u].visited = true;
        for (auto e : nodes[u].adj) {
            int v = e.dest;
            double w = e.distance;
            if (!nodes[v].visited && nodes[u].distance + w < nodes[v].distance) {
                nodes[v].distance = nodes[u].distance + w;
                auto aux = nodes[u].visitedAirports;
                aux.push(nodes[v].airport);
                nodes[v].visitedAirports = aux;
                q.decreaseKey(v, nodes[v].distance);
            }
        }
    }
    return nodes[dest];
}


// ------------------------------
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
    for (int i = 1; i <= size; i++)
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
    for (int i = 1; i <= size; i++)
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
    for (int i = 1; i <= size; i++)
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

    for (int i = 1; i <= size; i++)
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

    for (int i = 1; i <= size; i++)
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
 * Calculates the differente airlines that cooperate with an airport
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|E|)</b>, E -> number of edges
 * </pre>
 * @param i
 * @return set of all the differente airlines that work with a specific airport
 */
unordered_set<string> Graph::airlinesFromAirport(int i) {
    unordered_set<string> ans;
    for (const Edge& e : nodes[i].adj)
        ans.insert(e.airline.getCode());

    return ans;
}
unordered_set<pair<string, string>, Airport::CityHash, Airport::CityHash> Graph::targetsFromAirport(int i){
    unordered_set<pair<string,string>, Airport::CityHash, Airport::CityHash> ans;
    int count = 0;
    int repetidos = 0;
    for (const Edge& e : nodes[i].adj){
        int w = e.dest;
        if (ans.find({nodes[w].airport.getCountry(),nodes[w].airport.getCity()}) == ans.end()) count++;
        else repetidos ++;
        ans.insert({nodes[w].airport.getCountry(),nodes[w].airport.getCity()});
    }
    cout << count;
    return ans;
}
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
unordered_set<Airport, Airport::AirportHash, Airport::AirportHash> Graph::listAirports(int v, int max) {
    for (int i = 1; i <= size; i++) nodes[i].visited = false;
    unordered_set<Airport,Airport::AirportHash, Airport::AirportHash> airports;
    std::queue<int> q; // queue of unvisited nodes
    q.push(v);
    nodes[v].visited = true;
    nodes[v].nrFlights = 0;
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
unordered_set<pair<string,string>, Airport::CityHash, Airport::CityHash> Graph::listCities(int v, int max) {
    for (int i = 1; i <= size; i++) nodes[i].visited = false;
    unordered_set<pair<string,string>, Airport::CityHash, Airport::CityHash> cities;
    std::queue<int> q; // queue of unvisited nodes
    q.push(v);
    nodes[v].visited = true;
    nodes[v].nrFlights = 0;
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
set<string> Graph::listCountries(int v, int max) {
    for (int i = 1; i <= size; i++) nodes[i].visited = false;
    std::set<std::string> countries;
    std::queue<int> q; // queue of unvisited nodes
    q.push(v);
    nodes[v].visited = true;
    nodes[v].nrFlights = 0;
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

vector<string> Graph::getAirlines(int src, int dest,unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines) {
    vector<string> usedAirlines;
    for (auto e: nodes[src].adj)
        if (e.dest == dest && airlines.find(e.airline) == airlines.end())
            usedAirlines.push_back(e.airline.getCode());
    return usedAirlines;
}



void Graph::findPaths(vector<vector<int>>& paths,vector<int>& path, int v){

    if (v == -1) {
        if (find(paths.begin(),paths.end(),path) == paths.end())
            paths.push_back(path);
        return;
    }

    // Loop for all the parents
    // of the given vertex
    for (auto par : nodes[v].parents) {

        // Insert the current
        // vertex in path
        path.push_back(v);

        // Recursive call for its parent
        findPaths(paths,path, par);

        // Remove the current vertex
        path.pop_back();
    }
}

void Graph::bfs(int src, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines){
    // dist will contain shortest distance
    // from start to every other vertex
    for (int i = 1; i <= size; i++)
        nodes[i].distance = INT_MAX;
    queue<int> q;

    // Insert source vertex in queue and make
    // its parent -1 and distance 0
    q.push(src);
    nodes[src].parents = {-1};
    nodes[src].distance = 0;


    // Until Queue is empty
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto e : nodes[u].adj) {
            if (!airlines.empty() && airlines.find(e.airline) == airlines.end()) continue;
            int v = e.dest;
            if (nodes[v].distance > nodes[u].distance + 1) {

                // A shorter distance is found
                // So erase all the previous parents
                // and insert new parent u in parent[v]
                nodes[v].distance = nodes[u].distance + 1;

                q.push(v);
                nodes[v].parents.clear();
                nodes[v].parents.push_back(u);

            }
            else if (nodes[v].distance == nodes[u].distance + 1) {

                // Another candidate parent for
                // shortes path found
                nodes[v].parents.push_back(u);
            }
        }
    }
}


void Graph::printPaths(int start, int end, unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines) {
    vector<int> path;
    vector<vector<int> > paths;
    // Function call to bfs
    bfs(start,airlines);

    // Function call to find_paths
    findPaths(paths,path,end);
    if (paths.empty()){
        cout << "Não existem voos\n";
        return;
    }
    Supervisor supervisor;
    auto map = supervisor.getMap();
    for (auto v : paths) {

        // Since paths contain each
        // path in reverse order,
        // so reverse it
        reverse(v.begin(), v.end());
        vector<vector<string>> usedAirlines;
        // Print node for the current path
        for (int i = 0; i < v.size()-1; i++)
            usedAirlines.push_back(getAirlines(v[i],v[i+1],airlines));

        for (int i = 0; i < v.size()-1; i++) {
            auto possibleAirlines = getAirlines(v[i],v[i+1],airlines);
            printf("\033[1m\033[46m %s \033[0m", nodes[v[i]].airport.getCode().c_str());
            cout <<" --- (";
            for (int j = 0; j < possibleAirlines.size()-1; j++)
                printf("\033[1m\033[32m %s \033[0m |",possibleAirlines[j].c_str());
            printf("\033[1m\033[32m %s \033[0m",possibleAirlines[possibleAirlines.size()-1].c_str());


            //printf("\033[1m\033[32m %s \033[0m",airline.c_str());
            cout << ") --- ";

        }
        printf("\033[1m\033[46m %s \033[0m\n\n", nodes[v[v.size()-1]].airport.getCode().c_str());

    }
}
