//
// Created by Madalena Ye on 27/12/2022.
//

#ifndef RENAIR_MENU_H
#define RENAIR_MENU_H

#include "supervisor.h"

class Menu{
public:
    Menu();
    void init();
    void end();
private:
    void showOperations();
    void showInfo();
    void showStatistics();
    void showFlights();
    void showAirports();
    void showAirlines();
    void showCountries();
    int showTop();
    int customTop();
    void numberFlights();
    void numberAirports();
    string validateCountry(const string& message);
    string validateString(const string& message);
    Supervisor* supervisor;

};
#endif //RENAIR_MENU_H
