//
// Created by Madalena Ye on 27/12/2022.
//

#include "airline.h"
Airline::Airline(string code, string name, string callSign, string country){
    this->code= code;
    this->name= name;
    this->callSign=callSign;
    this->country=country;
}

Airline::Airline(string code){
    this->code= code;
}

string Airline::getCode() const {return this->code;}
string Airline::getName(){return this->name;}
string Airline::getCountry(){return this->country;}



