#include <iostream>
#include "classes/supervisor.h"

int main() {
    Supervisor supervisor;
    /*for (auto airline: supervisor.getAirports())
        cout << airline.getCode() << '\n';
    cout << supervisor.getAirports().size();*/
    Graph graph = supervisor.getGraph();
    cout << graph.distance(1,2);
    //createMenu();
    //endMenu();
}