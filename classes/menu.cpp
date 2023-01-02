//
// Created by Madalena Ye on 27/12/2022.
//
#include "menu.h"

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
                "[1] Realizar operação\n [2] Consultar informação\n [3] Ver estatísticas\n [0] Sair\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1 : showOperations(); break;
            case 2 : showInfo(); break;
            case 3 : showStatistics(); break;
            case 0 : return;
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
            "[1] Aeroportos\n [2] Companhias Aéreas\n [3] Países\n [0] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: showAirports(); break;
            case 2: showAirlines(); break;
            case 3: showCountries(); break;
            case 0: cout<< "\n"; return;
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
    while(true){
        cout << "\n Que dados pretende analisar? \n\n "
                "[1] Nº de voos\n [2] Nº de aeroportos \n [3] Nº de companhias\n "
                "[4] Nº de países atingíveis usando um máximo de Y voos\n "
                "[5] Nº de cidades atingíveis usando um máximo de Y voos\n [0] Voltar\n\n Opção: ";


        cin >> option;
        switch(option){
            case 1: numberFlights(); break;
            case 2: numberAirports(); break;
            case 3:  break;
            case 4: break;
            case 5: break;
            case 0: cout << "\n"; return;
        }
    }
}

void Menu::showAirports(){
    int option;
    while(true){
        cout << "\n Pretende ver os aeroportos:\n\n "
                "[1] Totais\n [2] De um país\n [3] Com mais voos\n [4] Com mais companhias aéreas\n [0] Voltar\n\n Opção: ";
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
                Graph graph = supervisor->getGraph();
                int option = showTop(), top;
                if (option == 1) top = 10;
                else if (option == 2) top = 20;
                else if (option == 3) top = customTop();
                else return;
                int j = 1;
                for (int i = 0; i < top; i++){
                    cout << "\n " << j << ". " << graph.flightsPerAirport()[i].second
                    << " - " << graph.flightsPerAirport()[i].first << " voos\n";
                    j++;
                }
                break;
            }
            case 4: {
                Graph graph = supervisor->getGraph();
                int option = showTop(), top;
                if (option == 1) top = 10;
                else if (option == 2) top = 20;
                else if (option == 3) top = customTop();
                else return;
                int j = 1;
                for (int i = 0; i < top; i++){
                    cout << "\n " << j << ". " << graph.airlinesPerAirport()[i].second
                         << " - " << graph.airlinesPerAirport()[i].first << " companhias aéreas\n";
                    j++;
                }
                break;
            }
            case 0: return;
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
                "[1] Totais\n [2] De um país\n [3] Com os voos mais longos\n [0] Voltar\n\n Opção: ";
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
            case 0: return;
            default: {
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
            }
        }
    }
}
void Menu::showCountries(){
    int option;
    while(true){
        cout << " Pretende ver os países:\n "
                "[1] Com mais aeroportos\n [2] Com mais companhias aéreas\n [0] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: break;
            case 2: break;
            case 0: return;
            default:{
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
            }
        }
    }
}
void Menu::numberFlights(){
    int option;
    while(true){
        cout << "\n Pretende saber quantos voos:\n\n "
                "[1] Existem no total\n [2] Existem num aeroporto\n [3] Existem num país\n [0] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: break;
            case 0: return;
            default:
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
        }
    }
}
void Menu::numberAirports() {
    int option;
    while(true){
        cout << "\n Pretende sabe quantos aeroportos:\n\n "
                "[1] Existem no total\n [2] Existem num país\n [3] São atingíveis usando um máximo de Y voos\n [0] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: cout << "\n Nº de aeroportos totais: " << supervisor->getAirports().size() << endl; break;
            case 2: {
                string country = validateCountry(" Insira o nome do país: ");
            }
        }
    }
}
string Menu::validateCountry(const string& message){
    string country;
    cout << message; cin >> country;
    while(cin.fail() || !supervisor->isCountry(country)) {
        cout << " Input inválido ou não existe" << '\n';
        cout << message;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> country;
    }
    return country;
}
string Menu::validateString(const string& message){
    string s;
    cout << message; cin >> s;
    while(cin.fail() || s.size() != 3) {
        cout << " Input inválido" << '\n';
        cout << message;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> s;
    }
    return s;
}
int Menu::showTop(){
    cout << "\n Deseja consultar:\n\n "
                "[1] Top 10\n [2] Top 20\n [3] Outro\n [0] Voltar\n\n Opção: ";
    int option;
    cin >> option;
    while (!(option == 1 || option == 2 || option == 3 || option == 0)){
        cout << " Input inválido\n Tente novamente: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> option;
    }
   return option;
}
int Menu::customTop() {
    cout << "\n Selecione um valor para o Top: ";
    int option; cin >> option;
    while (cin.fail() || option < 1 || option > 3020){
        cout << " Input entre 1 e 3020\n Tente novamente: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> option;
    }
    return option;
}
//close menu
void Menu::end() {
    printf("\n");
    printf("\033[46m===========================================================\033[0m\n");
}