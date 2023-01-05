#include <iostream>
//
// Created by Madalena Ye on 27/12/2022.
//
using namespace std;

#ifndef RENAIR_AIRLINE_H
#define RENAIR_AIRLINE_H
class Airline{
public:

    explicit Airline(string code);
    Airline(string code, string name, string callSign, string country);

    struct AirlineHash {
        // TODO
        int operator()(const Airline &b) const {
            string c = b.getCode();
            int v = 0;
            for (char i : c)
                v = 37 * v + i;
            return v;
        }

        // TODO
        bool operator()(const Airline &b1, const Airline &b2) const {
            return b1.getCode() == b2.getCode();
        }
    };

    string getCode() const;
    string getName();
    string getCountry();

private:
    string code;
    string name;
    string callSign;
    string country;
};
#endif //RENAIR_AIRLINE_H
