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
    void Operations();
    void chooseSource();
    void chooseTarget();
    void chooseAirlines();
    void processOperation();

    void showPath(list<queue<Airport>>, list<queue<Airline>>);
    void showInfo();
    void showAirports();
    void showAirlines();
    void showCountries();

    void showStatistics();

    int showTop();
    int customTop(int n);
    void numberFlights();
    void numberAirports();

    string validateCountry();
    int validateOption(const string& message);
    Airline validateAirline();
    double validateLatitude();
    double validateLongitude();
    double validateRadius();
    string validateCity(string country);
    string validateAirport();
    Supervisor* supervisor;
    vector<string> src;
    vector<string> dest;
    unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines;
};
#endif //RENAIR_MENU_H
