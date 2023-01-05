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
    static void end();
private:
    void operations();
    void chooseSource();
    void chooseTarget();
    void chooseAirlines();
    void processOperation();

    static void showPath(list<queue<Airport>>, list<queue<Airline>>);
    void showInfo();
    void showAirports();
    void showAirlines();
    void showCountries();
    void showStatistics();

    static int showTop();
    static int customTop(const string& message, int n);
    void numberFlights();
    void numberAirports();
    void numberAirlines();
    string validateCountry();
    static int validateOption(const string& message);
    string validateAirline();
    static double validateLatitude();
    static double validateLongitude();
    static double validateRadius();
    string validateCity(const string& country);
    string validateAirport();
    vector<string> validateLocal();
    Supervisor* supervisor;
    vector<string> src;
    vector<string> dest;
    unordered_set<Airline, Airline::AirlineHash, Airline::AirlineHash> airlines;
};
#endif //RENAIR_MENU_H
