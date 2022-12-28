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

string Airline::getCode() const {return this->code;}
string Airline::getName(){return this->name;}
string Airline::getCallSign(){return this->callSign;}
string Airline::getCountry(){return this->country;}

void Airline::setCode(string code){this->code=code;}
void Airline::setName(string name){this->name=name;}
void Airline::setCallSign(string callSign){this->callSign=callSign;}
void Airline::setCountry(string country){this->country=country;}
