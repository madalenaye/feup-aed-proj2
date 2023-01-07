#include "supervisor.h"
/**
 * Default Constructor for supervisor
 */
Supervisor::Supervisor() {
    createAirports();
    createAirlines();
    createGraph();
    countAirportsPerCountry();
}
unordered_map<string,int> Supervisor::getMap() const{
    return id_airports;
}

Graph Supervisor::getGraph() const {return graph;}
map<string,int> Supervisor::getNrAirportsPerCountry() const {return nrAirportsPerCountry;}
/**
 * Read airports.csv file and stores the airports information in id_city,id_airports,airports,cities,countries,
 * citiesPerCountry and in the graphs function we store the airports according to an index\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(n)</b>, n -> file lines
 * </pre>
 */
void Supervisor::createAirports() {
    ifstream myFile;
    string currentLine, code, name, city, country, x;
    double latitude, longitude;
    int i = 1;
    myFile.open("../data/airports.csv");
    getline(myFile, currentLine);

    while (getline(myFile,currentLine)){
        istringstream iss(currentLine);

        getline(iss,code,',');
        getline(iss,name,',');
        getline(iss,city,',');
        getline(iss,country,',');
        getline(iss,x,','); latitude = stod(x);
        getline(iss,x,','); longitude = stod(x);

        Airport airport = Airport(code,name,city,country,latitude,longitude);
        graph.addAirport(i,airport);
        id_city[{airport.getCountry(),airport.getCity()}].push_back(code);
        id_airports.insert({airport.getCode(),i++});
        airports.insert(airport);
        countries.insert(country);
        cities.insert(city);
        citiesPerCountry[country].push_back(city);
    }
}
/**
 * Read airlines.csv file and stores the information in airlines\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(n)</b>, n -> file lines
 * </pre>
 */
void Supervisor::createAirlines() {
    ifstream inFile;
    string code, name, callsign, country, line;
    inFile.open("../data/airlines.csv");
    getline(inFile, line);
    while(getline(inFile, line)){
        istringstream is(line);
        getline(is,code,',');
        getline(is,name,',');
        getline(is,callsign,',');
        getline(is,country,',');
        Airline a = Airline(code, name, callsign, country);
        airlines.insert(a);
    }
}
/**
 * Read flights.csv file and stores the airports information in graphs about the flights
 * (airport of departure/arrival and distance between them)\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(n*log(n))</b>, n -> file lines
 * </pre>
 */
void Supervisor::createGraph(){
    ifstream inFile;
    string source, target, airline, line;
    inFile.open("../data/flights.csv");
    getline(inFile, line);
    while(getline(inFile, line)){
        istringstream is(line);
        getline(is,source,',');
        getline(is,target,',');
        getline(is,airline,',');
        auto d = Graph::distance(airports.find(Airport(source))->getLatitude(),airports.find(Airport(source))->getLongitude()
                ,airports.find(Airport(target))->getLatitude(),airports.find(Airport(target))->getLongitude());
        graph.addEdge(id_airports[source],id_airports[target],Airline(airline),d);
    }
}
/**
 * Verifies if a country is in the countries unordered_set\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param country - wanted country
 * @return true if country exists in countries
 */
bool Supervisor::isCountry(const string& country){
    auto i = countries.find(country);
    if (i == countries.end()) return false;
    return true;
}
/**
 * Verifies if a airport is in the airports unordered_set\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param airport - wanted airport
 * @return true if airport exists in airports
 */
bool Supervisor::isAirport(const Airport& airport){
    auto i = airports.find(airport);
    if (i == airports.end()) return false;
    return true;
}
/**
 * Verifies if a airline is in the airline unordered_set\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param airline - wanted airline
 * @return true if airline exists in airlines
 */
bool Supervisor::isAirline(const Airline& airline) {
    auto i = airlines.find(airline);
    if (i == airlines.end()) return false;
    return true;
}
/**
 * Verifies if a city is in cities unordered_set\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @param city -city wanted to verify
 * @return true if city exists in cities
 */
bool Supervisor::isCity(const string& city) {
    auto i = cities.find(city);
    if (i == cities.end()) return false;
    return true;
}
/**
 * Verifies if a country and city is in citiesPerCountry unordered_map\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(n)</b> ,n -> number of cities from the specific country
 * </pre>
 * @param country - wanted country
 * @param city - wanted city
 * @return true if country,city exists
 */
bool Supervisor::isValidCity(const string& country, const string& city) {
    for (const auto& c : citiesPerCountry[country])
        if (city == c) return true;
    return false;
}
/**
 * Calculates the number of airports that exist in a radius of a certain coordinate\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V|*log(n))</b>,n -> number of airports that exist in a certain radius, V-> number of nodes
 * </pre>
 * @param latitude - latitude of the center of the circumference
 * @param longitude -longitude of the center of the circumference
 * @param radius - radius of the circumference
 * @return vector of codes of airports that exist in that range
 */
vector<string> Supervisor::localAirports(double latitude, double longitude, double radius) {
    vector<string> localAirports;
    double latitude1,longitude1;
    for (const auto& node : graph.getNodes()){
        latitude1 = node.airport.getLatitude();
        longitude1 = node.airport.getLongitude();
        if (Graph::distance(latitude,longitude,latitude1,longitude1) <= radius)
            localAirports.push_back(node.airport.getCode());
    }
    return localAirports;
}/**
 * Calculates the number of airlines that are founded in each country\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(n)</b>,n -> number of airlines
 * </pre>
 * @param country - country that the user wants to get information from
 * @return number of airlines
 */
int Supervisor::countAirlinesPerCountry(const string& country) {
    int count = 0;
    for (auto airline : airlines)
        if (airline.getCountry() == country)
            count++;
    return count;
}
/**
 * Calculates the number of airports that belong to each country\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(n*log(m))</b>,n -> number of airlines, m -> number of values in airportsPerCountry map
 * </pre>
 */
void Supervisor::countAirportsPerCountry() {
    map<string, int> airportsPerCountry;
    for (const auto& i : id_city){
        if (airportsPerCountry.find(i.first.first) == airportsPerCountry.end())
            airportsPerCountry[i.first.first] = i.second.size();
        else{
            auto m = airportsPerCountry.find(i.first.first);
            m->second+= i.second.size();
        }
    }
    nrAirportsPerCountry = airportsPerCountry;
}
/**
 * Swaps key and value.\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(1)</b>
 * </pre>
 * @tparam A -> key
 * @tparam B -> value
 * @param p
 * @return pair where key is the value(B) and value is the key(A)
 */
template<typename A, typename B>
pair<B,A> flip_pair(const pair<A,B> &p)
{
    return pair<B,A>(p.second, p.first);
}
/**
 * Transforms the map into a multimap and swaps it information\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(n*log(m))</b>,n -> size of map src, insertion of map src key in multimap
 * </pre>
 * @tparam A -> key
 * @tparam B -> value
 * @param src
 * @return flipped multimap were all the keys turn to values and values to keys
 */
template<typename A, typename B>
multimap<B,A> flip_map(const map<A,B> &src)
{
    multimap<B,A> dst;
    transform(src.begin(), src.end(), inserter(dst, dst.begin()),
                   flip_pair<A,B>);
    return dst;
}

multimap<int,string> Supervisor::convertMap(const map<string, int>& m) {
    return flip_map(m);
}
/**
 * Calculates the total number of flights\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(|V|)</b>, V -> number of nodes
 * </pre>
 * @return number of flights
 */
int Supervisor::nrFlights(){
    int nrFlights = 0;
    for (const auto& node: graph.getNodes())
        nrFlights += node.adj.size();
    return nrFlights;
}
/**
 * Calculates the smallest amount of flights possible to get to a specific airport from another airport\n\n
 * <b>Complexity\n</b>
 * <pre>
 *      <b>O(n*m*(|V|+|E|))</b>,n -> size of src vector, m -> size of dest vector,V -> number of nodes, E -> number of edges
 * </pre>
 * @param src  -> source node
 * @param dest -> final node
 * @param airlines -> airlines available for use
 * @return
 */
list<pair<string,string>> Supervisor::processFlight(vector<string> src, vector<string> dest,
                                                   Airline::AirlineH& airlines) {
    int bestFlight = INT_MAX;
    int nrFlights;
    string bestSource, bestTarget;
    list<pair<string,string>> res;
    for (const auto &s: src)
        for (const auto &d: dest) {
            if (src == dest) continue;

            nrFlights = graph.nrFlights(id_airports[s], id_airports[d], airline);

            if (nrFlights != 0 && nrFlights < bestFlight) bestFlight = nrFlights;
        }

    for (const auto &s: src)
        for (const auto &d: dest) {
            if (src == dest)
                continue;
            nrFlights = graph.nrFlights(id_airports[s], id_airports[d], airline);
            if (nrFlights  == bestFlight){
                res.emplace_back(s,d);
            }
        }
    return res;
}


list<pair<string,string>> Supervisor::processDistance(vector<string>& src, vector<string>& dest,
                                                    Airline::AirlineH& airlines) {

    double bestDistance = 99999999999999;
    double distance;
    string bestSource, bestTarget;
    list<pair<string,string>> res;
    for (const auto &s: src)
        for (const auto &d: dest) {
            if (src == dest)
                continue;
            distance = graph.flownDistance(id_airports[s], id_airports[d], airline);
            if (distance < bestDistance)
                bestDistance = distance;
        }

    for (const auto &s: src)
        for (const auto &d: dest) {
            if (s == d)
                continue;
            distance = graph.flownDistance(id_airports[s], id_airports[d], airline);
            if (bestDistance  == distance){
                res.emplace_back(s,d);
            }
        }
    return res;
}

