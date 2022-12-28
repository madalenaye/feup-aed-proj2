#include <iostream>

#include "classes/supervisor.h"
int main() {

    Supervisor supervisor;
    for (auto city:supervisor.getCity()){
        if (city.first.second=="London" ){
        cout << city.first.first<< '-' <<city.first.second << '-';
        for (auto i:city.second){
            cout << i << '\n';
        }}
    }
    //createMenu();
    //endMenu();
}
