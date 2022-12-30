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

    cout << "Source airports: \n";
    double latitude, longitude, radius;
    cout << " - Latitude: ";
    cin >> latitude;

    cout << " - Longitude: ";
    cin >> longitude;

    cout << " - Radius: ";
    cin >> radius;

    cout << "Target airports: \n";
    double latitude1, longitude1, radius1;
    cout << " - Latitude: ";
    cin >> latitude1;

    cout << " - Longitude: ";
    cin >> longitude1;

    cout << " - Radius: ";
    cin >> radius1;

    auto map = supervisor.getMap();
    auto airports = supervisor.getAirports();
    vector<string> airlines;
    airlines.push_back("TAP");
    airlines.push_back("AAL");
    airlines.push_back("BAW");
    airlines.push_back("EZY");


    //auto dist = graph.distance(airports.find(Airport(src))->getLatitude(),airports.find(Airport(src))->getLongitude(),airports.find(Airport(dest))->getLatitude(),airports.find(Airport(dest))->getLongitude());
    //cout << dist;
    vector<string> source, dest;

    for (auto a: airports) {
        if (graph.distance(a.getLatitude(), a.getLongitude(), latitude, longitude) <= radius) {
            source.push_back(a.getCode());
        }
    }
    cout << "Source: ";
    for (auto a : source){
        cout << a << " - ";
    }

    cout << '\n';

    for (auto a: airports){
        if (graph.distance(a.getLatitude(),a.getLongitude(),latitude1,longitude1) <= radius1){
            dest.push_back(a.getCode());
        }
    }
    cout << "Target: ";
    for (auto a : dest){
        cout << a << " - ";
    }

    cout << '\n';


    for (auto x : source){
        for (auto y: dest){
            if (x!=y){
                cout << "Flight " << x << " to " << y << " : ";
                auto vetor = graph.bfs(map[x],map[y],airlines);
                if (vetor.second.empty()) cout << "Not found\n";

                else {
                    for (int i = 0; i < vetor.second.size() - 1; i++) {
                        cout << vetor.second[i].getCode() << " --- ( ";
                        cout << vetor.first[i] << " ) --> ";
                    }
                    cout << vetor.second[vetor.second.size() - 1].getName() << '\n';
                    cout << "Number of flights : " << vetor.second.size() - 1 << '\n';
                }
            }
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