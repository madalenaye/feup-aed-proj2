#ifndef MENU_H
#define MENU_H

#include "supervisor.h"

class Menu{
public:
    Menu();
    void init();
    static void end();
private:
    void chooseSource();
    void chooseTarget();
    void chooseAirlines(bool op);
    void processOperation();

    void info();
    void showAirport();
    void showOptions(const string& airport);
    void showAirports();
    void showAirlines();
    void showCountries();

    void statistics();
    void numberFlights();
    void numberAirports();
    void numberAirlines();
    void airportStats();
    void maxReach();

    static int showTop();
    static int customTop(const string& message, int n);

    string validateAirport();
    string validateAirline();
    string validateCountry();
    string validateCity(const string& country);
    static double validateLatitude();
    static double validateLongitude();
    static double validateRadius();
    vector<string> validateLocal();
    static string validateOption(const string& message);

    Supervisor* supervisor;
    vector<string> src;
    vector<string> dest;
    Airline::AirlineH airlines;
    
};
#endif //MENU_H
