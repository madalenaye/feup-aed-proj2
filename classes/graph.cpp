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
 * Adds an edge from source to destination with an airline and distance\n\n
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
 * Adds an airport to a node\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param src - source node
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
 * Calculates the distance between two points on a sphere using their latitudes and longitudes measured along the surface\n\n
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
 * Calculates the number of flights of a specific airline\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|E|)</b>, E -> number of edges
 * </pre>
 * @param airline
 * @return number of flights of a specific airline
 */
int Graph::airlineFlights(const string& airline){
    int count = 0;
    for (int i = 1 ; i <= size; i++)
        for (const Edge& e : nodes[i].adj)
            if (e.airline.getCode() == airline)
                count++;
    return count;
}

bool cmp( const pair<int,string>& a, const pair<int,string>& b){
    return a.first > b.first;
}

/**
 * Calculates the number of departures of each airport.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V|*log(|V|))</b>, V -> number of nodes
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
 * Calculates the number of airlines that work with each airport.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E| + n * log(n))</b>, V -> number of nodes, E -> number of edges, n -> size of nrAirlines
 * </pre>
 * @return ordered vector of pair<Number of airlines,Airport Code> by descending order of number of airlines
 */
vector<pair<int,string>> Graph::airlinesPerAirport() {

    vector<pair<int,string>> nrAirlines;

    for (int i = 1; i <= size; i++){
        set<string> n;

        for (const Edge& e : nodes[i].adj)
            n.insert(e.airline.getCode());

        nrAirlines.emplace_back(n.size(), nodes[i].airport.getCode());
    }

    sort(nrAirlines.begin(), nrAirlines.end(), cmp);

    return nrAirlines;
}

/**
 * Calculates the reachable airports using "max" number of flights\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param v - source node
 * @param max - number of flights
 * @return set of reachable airports using "max" number of flights
 */
Airport::AirportH Graph::listAirports(int v, int max) {

    for (int i = 1; i <= size; i++)
        nodes[i].visited = false;

    Airport::AirportH airports;

    queue<int> q;
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
                if (nodes[w].distance<=max) airports.insert(nodes[w].airport);
            }
        }
    }
    return airports;
}

/**
 * Calculates the reachable cities using "max" number of flights\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param v - source node
 * @param max - number of flights
 * @return unordered_set of reachable cities using "max" number of flights
 */
Airport::CityH2 Graph::listCities(int v, int max) {
    for (int i = 1; i <= size; i++)
        nodes[i].visited = false;

    Airport::CityH2 cities;

    queue<int> q;
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
                if (nodes[w].distance <= max) cities.insert({nodes[w].airport.getCountry(),nodes[w].airport.getCity()});
            }
        }
    }
    return cities;
}

/**
 * Calculates the reachable countries using "max" number of flights\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V| + |E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param v - source node
 * @param max - number of flights
 * @return set of reachable countries using "max" number of flights
 */
set<string> Graph::listCountries(int v, int max) {
    for (int i = 1; i <= size; i++)
        nodes[i].visited = false;

    set<string> countries;

    queue<int> q;
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
                if (nodes[w].distance <= max) countries.insert(nodes[w].airport.getCountry());
            }
        }
    }
    return countries;
}

/**
 * From a specific airport, calculates all of the airports that are reachable within 1 flight.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|E|)</b>,E -> number of edges of source node
 * </pre>
 * @param source -> source node
 * @return unordered_set of airports code and name
 */
Graph::PairH Graph::airportsFromAirport(int source) {
    Graph::PairH ans;
    for(const auto& e:nodes[source].adj)
        ans.insert({nodes[e.dest].airport.getCode(),nodes[e.dest].airport.getName()});
    return ans;
}

/**
 * Calculates the different airlines that cooperate with an airport\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|E|)</b>, E -> number of edges of node i
 * </pre>
 * @param i -> source node
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
 *      <b>O(|E|)</b>, E -> number of edges of node i
 * </pre>
 * @param i -> source node
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
 *      <b>O(|E|)</b>, E -> number of edges of node i
 * </pre>
 * @param i -> source node
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
 * Stores in the parents variable the possible flight candidates using bfs. Parents who also have possible flight candidates
 * allowing us to get all the possible flights from a certain source.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V|+|E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param src -> source node
 * @param airlines -> unordered set of airlines to use (if empty, use all airlines)
 */
void Graph::bfsPath(int src, Airline::AirlineH airlines){

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
        for (const auto& e : nodes[u].adj) {
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

/**
 * Stores in paths all possible paths to node v.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(n)</b> n -> paths size
 * </pre>
 * @param paths -> vector of paths that are possible
 * @param path -> current path
 * @param v -> target node
 */
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

/**
 * Calculates the minimum flown distance between source airport and target airport using airlines \n \n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(log(|V|))</b>, V -> number of nodes
 * </pre>
 * @param src - source node / node of source airport
 * @param dest - target node
 * @param airlines - unordered set of airlines to use (if empty, use all airlines)
 * @return minimum flown distance between source airport and target airport using airlines
 */
Graph::Node Graph::dijkstra(int src, int dest, Airline::AirlineH airlines) {

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

        for (const auto& e : nodes[u].adj) {

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

/**
 * Calculates the max distance between connected nodes\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V|+|E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @param v -> source node
 * @return the diameter of a connected component
 */
double Graph::bfsDiameter(int v) {
    for (int i = 1; i <= size; i++){
        nodes[i].visited = false;
        nodes[i].distance = -1.0;
    }

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

/**
 * Calculates the diameter using a bfs.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V|+|E|)</b>, V -> number of nodes, E -> number of edges
 * </pre>
 * @return diameter between all connected components.
 */
double Graph::diameter() {
    nodes[1].visited = true;
    double max = bfsDiameter(1);
    for (int i = 1; i <= size; i++)
        if (!nodes[i].visited){
            nodes[i].visited = true;
            double diameter = bfsDiameter(i);
            if (diameter > max) max = diameter;
        }
    return max;
}

/**
 * Finds the nodes that are articulation points and inserts them in res\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O((|V| + |E|)*n)</b>, V -> number of nodes, E -> number of edges, n-> list size
 * </pre>
 * @param v -> source node
 * @param index
 * @param res -> list of articulation points
 * @param airlines -> unordered set of airlines to use (if empty, use all airlines)
 */
void Graph::dfsArt(int v, int index, list<int>& res,Airline::AirlineH airlines) {
    nodes[v].num = nodes[v].low = index++;
    nodes[v].art = true;
    int count = 0;
    for (const auto& e : nodes[v].adj){
        auto w = e.dest;
        if(airlines.find(e.airline) != airlines.end() || airlines.empty()){
            if (nodes[w].num == 0){
                count++;
                dfsArt(w,index,res,airlines);
                nodes[v].low = min(nodes[v].low,nodes[w].low);
                if (nodes[w].low >= nodes[v].num && std::find(res.begin(),res.end(),v) == res.end()) {
                    if (index == 2 && count > 1) res.push_back(1);
                    else if (index != 2 && std::find(res.begin(),res.end(),v)== res.end()) res.push_back(v);
                }
            }
            else if (nodes[v].art)
                nodes[v].low = min(nodes[v].low, nodes[w].num);
        }
    }
}

/**
 * Calculates the list of articulation points that exist in a specific unordered_set of airlines or in all airlines.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O((|V| + |E|)*n)</b>, V -> number of nodes, E -> number of edges, n-> list size
 * </pre>
 * @param airlines -> unordered set of airlines to use (if empty, use all airlines)
 * @return The list of articulation points.
 */
list<int> Graph::articulationPoints(const Airline::AirlineH& airlines) {
    list<int> answer;

    for (int i = 1; i <= size; i++)
        nodes[i].visited = nodes[i].art = false;

    int index = 1;

    for (int i = 1; i <= size; i++)
        if (nodes[i].num == 0)
            dfsArt(i,index,answer,airlines);

    return answer;
}

/**
 * Searches all the airlines that can be used to travel between a source and dest with a certain user input of airlines(or none).\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|E|)</b> E -> number of edges of source node
 * </pre>
 * @param src -> source node
 * @param dest -> final node
 * @param airlines -> unordered_set of airlines that without user input is empty, if has user input only uses those specific airlines.
 * @return vector of the possible airlines to use to travel from src to dest
 */
vector<string> Graph::getAirlines(int src, int dest, Airline::AirlineH airlines) {
    vector<string> usedAirlines;
    for (const auto& e: nodes[src].adj)
        if (e.dest == dest && (airlines.empty() || airlines.find(e.airline) != airlines.end()))
            usedAirlines.push_back(e.airline.getCode());
    return usedAirlines;
}

/**
 * Prints a possible path from a source airport to a target airport\n\n
 *  * <b>Complexity\n</b>
 * <pre>
 *      <b>O(n*m)</b>, n -> path size , m -> possibleAirlines size
 * </pre>
 * @param path -> visited nodes during the path
 * @param airlines -> unordered set of airlines to use (if empty, use all airlines)
 */
void Graph::printPath(vector<int> path, const Airline::AirlineH& airlines) {
    for (int i = 0; i < path.size()-1; i++){
        auto possibleAirlines = getAirlines(path[i],path[i+1],airlines);
        printf("\033[1m\033[46m %s \033[0m", nodes[path[i]].airport.getCode().c_str());
        cout <<" --- (";
        for (int j = 0; j < possibleAirlines.size()-1; j++)
            printf("\033[1m\033[32m %s \033[0m |",possibleAirlines[j].c_str());
        printf("\033[1m\033[32m %s \033[0m",possibleAirlines[possibleAirlines.size()-1].c_str());
        cout << ") --- ";
    }
    printf("\033[1m\033[46m %s \033[0m\n\n", nodes[path[path.size()-1]].airport.getCode().c_str());
}

/**
 * Calculates (using bfs) and prints most optimal path of flights(least amount of flights)\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O((|V|+|E|)*p)</b>, V -> number of nodes , E-> number of edges, p-> possibleAirlines size
 * </pre>
 * @param nrPath
 * @param start -> source node
 * @param end -> final node
 * @param airlines -> unordered set of airlines to use (if empty, use all airlines)
 */
void Graph::printPathsByFlights(int& nrPath, int start, int end, const Airline::AirlineH& airlines) {
    vector<int> path;
    vector<vector<int> > paths;

    bfsPath(start,airlines);
    findPaths(paths,path,end);

    Supervisor supervisor;
    auto map = supervisor.getMap();
    for (auto v : paths) {
        reverse(v.begin(), v.end());
        cout << " Trajeto nº" << ++nrPath << ": ";
        printPath(v,airlines);
    }
}

/**
 * Calculates and prints the most optimal paths based on distance of nodes using the dijkstra algorithm.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(log(|V|)*p)</b>, V -> number of nodes , p -> possibleAirlines size
 * </pre>
 * @param nrPath
 * @param start -> source node
 * @param end  -> final node
 * @param airlines -> unordered set of airlines to use (if empty, use all airlines)
 */
void Graph::printPathsByDistance(int& nrPath, int start, int end, const Airline::AirlineH& airlines) {
    Node node = dijkstra(start,end,airlines);

    if (node.parents.empty()) {
        cout << " Não existem voos\n\n";
        return;
    }

    cout << " Trajeto nº" << ++nrPath << ": ";
    printPath(node.parents,airlines);

}


