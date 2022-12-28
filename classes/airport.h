//
// Created by Madalena Ye on 27/12/2022.
//

#ifndef RENAIR_AIRPORT_H
#define RENAIR_AIRPORT_H

#include <string>

using namespace std;

class Airport {
public:
    Airport();
    Airport(string, string, string, string, double, double);

    string getCode() const;
    string getName();
    string  getCity() const;
    string getCountry() const;
    double getLatitude();
    double getLongitude();

    void setCode(string);
    void setName(string);
    void setCity(string);
    void setCountry(string);
    void setLatitude(double);
    void setLongitude(double);

private:
    string code;
    string name;
    string city;
    string country;
    double latitude;
    double longitude;
};

#endif //RENAIR_AIRPORT_H
