//
// Created by Madalena Ye on 27/12/2022.
//

#ifndef RENAIR_AIRPORT_H
#define RENAIR_AIRPORT_H

#include <string>

using namespace std;

class Airport {
public:
    explicit Airport(string);
    Airport(string, string, string, string, double, double);

    string getCode() const;
    string getName() const;
    string getCity() const;
    string getCountry() const;
    double getLatitude() const;
    double getLongitude() const;

    struct AirportHash {
        // TODO
        int operator()(const Airport &b) const {
            string c = b.getCode();
            int v = 0;
            for (char i : c)
                v = 37 * v + i;
            return v;
        }

        // TODO
        bool operator()(const Airport &b1, const Airport &b2) const {
            return b1.getCode() == b2.getCode();
        }
    };

private:
    string code;
    string name;
    string city;
    string country;
    double latitude{};
    double longitude{};
};

#endif //RENAIR_AIRPORT_H
