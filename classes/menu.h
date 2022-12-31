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
    void showAirports();
    void showAirlines();
    void showTarget();
    string validateCountry(string message);
    string validateAirline(string message);
    Supervisor* supervisor;

};
#endif //RENAIR_MENU_H
