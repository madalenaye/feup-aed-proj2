#include <iostream>
//
// Created by Madalena Ye on 27/12/2022.
//
using namespace std;

#ifndef RENAIR_AIRLINE_H
#define RENAIR_AIRLINE_H
class Airline{
public:
    Airline(string code);
    Airline(string code, string name, string callSign, string country);
    string getCode() const;
    string getName();
    string getCallSign();
    string getCountry();
    void setCode(string);
    void setName(string);
    void setCallSign(string);
    void setCountry(string);
    struct AirlineHash {
        // TODO
        int operator()(const Airline &b) const {
            string c = b.getCode();
            int v = 0;
            for (unsigned int i = 0; i < c.size(); i++)
                v = 37 * v + c[i];
            return v;
        }

        // TODO
        bool operator()(const Airline &b1, const Airline &b2) const {
            return b1.getCode() == b2.getCode();
        }
    };
private:
    string code;
    string name;
    string callSign;
    string country;
};
#endif //RENAIR_AIRLINE_H
