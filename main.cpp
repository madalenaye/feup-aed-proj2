#include <iostream>
#include "classes/supervisor.h"

int main() {
    Supervisor supervisor;
    Graph graph = supervisor.getGraph();
    auto x = supervisor.getAirports();
    auto z = Airport("CDG");
    auto y = x.find(z);
    if (y == x.end()) cout << "Not found\n";
    else cout << y->getName()<< '\n';
    string src, dest;
    cout << "Source: ";
    cin >> src;

    cout << "Target: ";
    cin >> dest;
    auto map = supervisor.getMap();


    auto vetor = graph.distance(map[src],map[dest]);
    for (int i = 0; i<vetor.size()-1; i++){
        cout << vetor[i].getName() << " - ";
    }
    cout << vetor[vetor.size()-1].getName() << '\n';
    cout << "Number of flights : " << vetor.size()-1;
    //createMenu();
    //endMenu();
}