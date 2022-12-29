#include <iostream>
#include "classes/supervisor.h"

int main() {
    Supervisor supervisor;
    Graph graph = supervisor.getGraph();

    string src, dest;
    cout << "Source: ";
    cin >> src;

    cout << "Target: ";
    cin >> dest;
    auto map = supervisor.getMap();

    vector<string> airlines;
    airlines.push_back("TAP");
    airlines.push_back("AAL");
    airlines.push_back("BAW");

    auto vetor = graph.distance(map[src],map[dest],airlines);
    if (vetor.second.empty()) cout << "Not found\n";

    else {
        for (int i = 0; i < vetor.second.size() - 1; i++) {
            cout << vetor.second[i].getCode() << " --- ( ";
            cout << vetor.first[i] << " ) --> ";
        }
        cout << vetor.second[vetor.second.size() - 1].getName() << '\n';
        cout << "Number of flights : " << vetor.second.size() - 1;


    }
    //createMenu();
    //endMenu();
}