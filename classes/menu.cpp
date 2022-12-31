//
// Created by Madalena Ye on 27/12/2022.
//
#include "Menu.h"

//initial menu
Menu::Menu() {
    printf("\n");
    printf("\033[44m===========================================================\033[0m\t\t");
    cout << "\n\n" << " Bem-vindo!" << endl;
    supervisor = new Supervisor();
}

//options of the menu
void Menu::init() {
    int option;
    while(true) {
        cout << " O que deseja fazer hoje?\n\n "
                "[1] Realizar operação\n [2] Consultar informação\n [3] Ver estatísticas\n [4] Sair\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1 : showOperations(); break;
            case 2 : showInfo(); break;
            case 3 : showStatistics(); break;
            case 4 : end(); return;
            default: {
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
            }
        }
    }
}

void Menu::showOperations() {
    cout << "standby";
}
void Menu::showInfo(){
    int option;
    while(true){
        cout << "\n Que tipo de informação deseja ver? \n\n "
            "[1] Aeroportos\n [2] Companhias Aéreas\n [3] Destinos\n [4] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: showAirports(); break;
            case 2: showAirlines(); break;
            case 3: showTarget(); break;
            case 4: cout<< "\n"; return;
            default:{
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
            }
        }
    }
}
void Menu::showStatistics() {
    int option;
    cout << "\n Que dados pretende analisar? \n\n "
            "[1] Nº de voos\n [2] Nº de aeroportos \n [3] Nº de companhias\n "
            "[4] Nº de países atingíveis usando um máximo de Y voos\n "
            "[5] Nº de cidades atingíveis usando um máximo de Y voos\n\n Opção: ";

    /*while(true){
        cin >> option;
        switch(option){
            case 1: listAirports(); break;
            case 2: showCompanies(); break;
            case 3: showAirports(); break;
        }
    }*/
}

void Menu::showAirports(){
    int option;
    while(true){
        cout << "\n Pretende ver os aeroportos:\n\n "
                "[1] Totais\n [2] De um país\n [3] Com mais voos\n [4] Com mais companhias aéreas\n [5] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: {
                for (auto i: supervisor->getAirports()) cout << " " << i.getCode() << " - " << i.getName() << endl;
                break;
            }
            case 2: {
                string country = validateCountry(" Insira o nome do país (ex: Portugal): ");
                cout << "\n";
                for (auto i : supervisor->getAirports()){
                    if (i.getCountry() == country) cout << " " << i.getCode() << " - " << i.getName() << endl;
                }
                break;
            }
            case 3: {
                //standby
                break;
            }
            case 4: {
                //standby
                /*
                string company = validateAirline(" Insira o código de uma companhia aérea (ex: RYR): ");
                for (char& c: company) c = toupper(c);
                for (auto i : supervisor->getAirports()){
                }*/
                break;
            }
            case 5: return;
            default: {
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
            }

        }
    }
}
void Menu::showAirlines(){
    int option;
    while(true){
        cout << "\n Pretende ver as companhias aéreas:\n\n "
                "[1] Totais\n [2] De um país\n [3] Com os voos mais longos\n [4] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: {
                for (auto i : supervisor->getAirlines()) cout << " " << i.getCode() << " - " << i.getName() << endl;
                break;
            }
            case 2: {
                string country = validateCountry(" Insira o nome do país (ex: Portugal): ");
                cout << "\n";
                for (auto i : supervisor->getAirlines()){
                    if (i.getCountry() == country){ cout << " " << i.getCode() << " - " << i.getName() << endl;}
                }
                break;
            }
            case 3: break; //standby
            case 4: return;
            default: {
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
            }
        }
    }
}
void Menu::showTarget(){
    int option;
    while(true){
        cout << " Pretende ver os destinos:\n "
                "[1] Mais populares para visitar\n [2] Menos populares para visitar\n [3] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: break;
            case 2: break;
            case 3: return;
            default:{
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
            }
        }
    }
}
string Menu::validateCountry(string message){
    string country;
    cout << message; cin >> country;
    while(cin.fail() || !supervisor->isCountry(country)) {
        cout << " Input inválido" << '\n';
        cout << " Insira o nome do país: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> country;
    }
    return country;
}
string Menu::validateAirline(string message){
    string airline;
    cout << message; cin >> airline;
    while(cin.fail() || airline.size() != 3) {
        cout << " Input inválido" << '\n';
        cout << " Insira o código da companhia aérea: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> airline;
    }
    return airline;
}
//close menu
void Menu::end() {
    printf("\n");
    printf("\033[46m===========================================================\033[0m\n");
}