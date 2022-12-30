#include <iostream>
#include "classes/supervisor.h"

int main() {
    Supervisor supervisor;
    Graph graph = supervisor.getGraph();
    /*
    string src, dest;
    cout << "Source: ";
    cin >> src;

    cout << "Target: ";
    cin >> dest;*/

    double latitude, longitude, radius;
    cout << "Latitude: ";
    cin >> latitude;

    cout << "Longitude: ";
    cin >> longitude;

    cout << "Radius: ";
    cin >> radius;

    auto map = supervisor.getMap();
    auto airports = supervisor.getAirports();
    vector<string> airlines;
    airlines.push_back("TAP");
    airlines.push_back("AAL");
    airlines.push_back("BAW");


    //auto dist = graph.distance(airports.find(Airport(src))->getLatitude(),airports.find(Airport(src))->getLongitude(),airports.find(Airport(dest))->getLatitude(),airports.find(Airport(dest))->getLongitude());
    //cout << dist;

    cout << "Airports:\n";
    for (auto a: airports){
        if (graph.distance(a.getLatitude(),a.getLongitude(),latitude,longitude) <= radius){
            cout << a.getCode() << '\n';
        }
    }
    /*
    auto vetor = graph.bfs(map[src],map[dest],airlines);
    if (vetor.second.empty()) cout << "Not found\n";

    else {
        for (int i = 0; i < vetor.second.size() - 1; i++) {
            cout << vetor.second[i].getCode() << " --- ( ";
            cout << vetor.first[i] << " ) --> ";
        }
        cout << vetor.second[vetor.second.size() - 1].getName() << '\n';
        cout << "Number of flights : " << vetor.second.size() - 1;
    }
     */
    //createMenu();
    //endMenu();
}