#include <iostream>

#include "classes/supervisor.h"
#include "classes/menu.h"

int main() {
    /*
    Supervisor supervisor;
    for (auto airline: supervisor.getAirports()){
        cout << airline.getCode() << '\n';

    }
    cout << supervisor.getAirports().size();*/
    Menu menu;
    menu.init();
    return 0;
}
