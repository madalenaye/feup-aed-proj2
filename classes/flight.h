//
// Created by Madalena Ye on 31/12/2022.
//

#ifndef RENAIR_FLIGHT_H
#define RENAIR_FLIGHT_H

#include <iostream>
using namespace std;

class Flight{
public:
    Flight();
    Flight(string, string, string);
    string getSource(){return source;}
    string getTarget(){return target;}
    string getAirline(){return airline;}
private:
    string source;
    string target;
    string airline;
};
#endif //RENAIR_FLIGHT_H
