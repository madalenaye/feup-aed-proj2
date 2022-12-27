#include <iostream>
//
// Created by Madalena Ye on 27/12/2022.
//
using namespace std;

#ifndef RENAIR_AIRLINE_H
#define RENAIR_AIRLINE_H
class Airline{
    public:
        Airline();
        Airline(string code, string name, string callSign, string country);
        string getCode();
        string getName();
        string getCallSign();
        string getCountry();
        void setCode(string);
        void setName(string);
        void setCallSign(string);
        void setCountry(string);
        bool operator=(const Airline& airline) const;
    private:
        string code;
        string name;
        string callSign;
        string country;
};
#endif //RENAIR_AIRLINE_H
