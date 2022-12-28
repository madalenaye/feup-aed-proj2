#include <iostream>
#include "classes/supervisor.h"

int main() {
    Supervisor supervisor;
    /*for (auto airline: supervisor.getAirports())
        cout << airline.getCode() << '\n';
    cout << supervisor.getAirports().size();*/
    Graph graph = supervisor.getGraph();

    string src, dest;
    cout << "Source: ";
    cin >> src;

    cout << "Target: ";
    cin >> dest;
    auto map = supervisor.getMap();


    cout << graph.distance(map[src],map[dest]);
    //createMenu();
    //endMenu();
}