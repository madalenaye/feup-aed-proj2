#ifndef AIRPORT_H
#define AIRPORT_H

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
        int operator()(const Airport &b) const {
            string c = b.getCode();
            int v = 0;
            for (char i : c) v = 37 * v + i;
            return v;
        }
        bool operator()(const Airport &b1, const Airport &b2) const {
            return b1.getCode() == b2.getCode();
        }
    };

    struct CityHash{
        int operator()(const pair<string,string> &b) const {
            string cityCode = b.second;
            int v = 0;
            for (char i : cityCode) v = 37 * v + i;
            return v;
        }
        bool operator()(const pair<string,string> &b1, const pair<string,string> &b2) const {
            return b1.first == b2.first && b1.second== b2.second;
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

#endif //AIRPORT_H
