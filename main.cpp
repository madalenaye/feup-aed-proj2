#include <iostream>

#include "classes/supervisor.h"
int main() {

    Supervisor supervisor;
    for (auto airline: supervisor.getAirlines()){
        cout << airline.getCode() << '\n';

    }
    cout << supervisor.getAirlines().size();
    //createMenu();
    //endMenu();
}
