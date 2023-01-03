#include <iostream>
#include "classes/supervisor.h"
#include <map>
#include "classes/menu.h"
using namespace std;



int main() {
    Menu menu;
    menu.init();
    menu.end();
    /*Supervisor supervisor;
    multimap<int, string> dst = supervisor.countAirportsPerCountry();
    cout << dst.size();*/
    return 0;
}
