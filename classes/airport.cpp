//
// Created by Madalena Ye on 27/12/2022.
//

#include "airport.h"


Airport::Airport(string code, string name, string city, string country, double latitude, double longitude) {
    this->code = code;
    this->name = name;
    this->city = city;
    this->country = country;
    this->latitude = latitude;
    this->longitude = longitude;
}

string Airport::getCode() const {return code;}
string Airport::getName() {return name;}
string Airport::getCity() {return city;}
string Airport::getCountry() {return country;}
double Airport::getLatitude() {return latitude;}
double Airport::getLongitude() {return longitude;}

void Airport::setCode(string code) {this->code = code;}
void Airport::setName(string name) {this->name = name;}
void Airport::setCity(string city) {this->city = city;}
void Airport::setCountry(string country) {this->country = country;}
void Airport::setLatitude(double latitude) {this->latitude = latitude;}
void Airport::setLongitude(double longitude) {this->longitude = longitude;}
