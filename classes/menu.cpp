//
// Created by Madalena Ye on 27/12/2022.
//
#include <climits>
#include "menu.h"

//initial menu
Menu::Menu() {
    printf("\n");
    printf(" \033[44m===========================================================\033[0m\t\t");
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
            case 1 : Operations(); break;
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

void Menu::Operations() {
    chooseSource();
    chooseTarget();
    processOperation();
}

void Menu::chooseSource() {
    int option;
    while(true){
        cout << "\n Pretende partir de: \n\n "
                "[1] Um aeroporto em específico\n [2] Uma cidade\n [3] Uma localização\n [0] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: {
                string airport = validateAirport();
                src.push_back(airport);
                return;
            }
            case 2: {
                string country = validateCountry();
                string city = validateCity(country);
                auto airportsPerCity = supervisor->getCity();
                src = airportsPerCity[{country,city}];
                return;
            }
            case 3: {
                double latitude = validateLatitude();
                double longitude = validateLongitude();
                double radius = validateRadius();
                this->src = supervisor->localAiports(latitude,longitude,radius);
                while (dest.empty()){
                    cout << " Não existem aeroportos no local indicado, tente novamente\n";
                    latitude = validateLatitude();
                    longitude = validateLongitude();
                    radius = validateRadius();
                    dest = supervisor->localAiports(latitude,longitude,radius);
                }
                return;
            }
            case 0: cout << "\n"; init(); return;
            default:{
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
            }
        }
    }
}
void Menu::chooseTarget() {
    int option;
    while(true){
        cout << "\n Pretende chegar a: \n\n "
                "[1] Um aeroporto em específico\n [2] Uma cidade\n [3] Uma localização\n [0] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: {
                string destination = validateAirport();
                while (src.size()==1 && destination == src.front()) {
                    cout << " Não pode partir e chegar ao mesmo aeroporto num voo :(\n";
                    destination = validateAirport();
                }
                this->dest.push_back(destination);

                int choice = validateOption("\n Pretende escolher as companhias aéreas a usar? \n\n"
                                            " [1] Sim\n [2] Não\n\n Opção: ");
                if (choice == 1) chooseAirlines();
                return;
            }
            case 2: {
                string country = validateCountry();
                string city = validateCity(country);
                auto airportsPerCity = supervisor->getCity();
                dest = airportsPerCity[{country,city}];
                int choice = validateOption("\n Pretende escolher as companhias aéreas a usar? \n\n"
                                            " [1] Sim\n [2] Não\n\n Opção: ");
                if (choice == 1) chooseAirlines();
                return;
            }
            case 3: {
                double latitude = validateLatitude();
                double longitude = validateLongitude();
                double radius = validateRadius();
                dest = supervisor->localAiports(latitude,longitude,radius);
                while (dest.empty()){
                    cout << " Não existem aeroportos no local indicado, tente novamente\n";
                    latitude = validateLatitude();
                    longitude = validateLongitude();
                    radius = validateRadius();
                    dest = supervisor->localAiports(latitude,longitude,radius);
                }
                int choice = validateOption("\n Pretende escolher as companhias aéreas a usar? \n\n"
                                            " [1] Sim\n [2] Não\n\n Opção: ");
                if (choice == 1) chooseAirlines();
                return;
            }
            case 0: cout<< "\n"; Operations(); return;
            default:{
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
            }
        }
    }
}
void Menu::chooseAirlines() {

    Airline airline = validateAirline();
    airlines.insert(airline);

    int option = validateOption("\n Deseja inserir mais alguma companhia aérea? \n\n"
                                " [1] Sim\n [2] Não\n\n Opção: ");;
    while (option == 1){
        airline = validateAirline();
        airlines.insert(airline);
        option = validateOption("\n Deseja inserir mais alguma companhia aérea? \n\n"
                                " [1] Sim\n [2] Não\n\n Opção: ");
    }
}
void Menu::processOperation() {
    Graph graph = supervisor->getGraph();
    auto map = supervisor->getMap();
    int option = validateOption("\n Indique o critério a usar: \n\n"
                                " [1] Número mínimo de voos\n [2] Distância mínima percorrida\n\n Opção: ");;
    if (option == 1){
        for (auto s : src)
            for (auto d : dest){
                printf("\n\n \033[40m===========================================================\033[0m\n\n");
                if (graph.nrFlights(map[s],map[d],airlines) == -1)
                    cout << " Não existem voos de " << s << " a " << d << " com as companhias aéreas indicadas\n";
                else {
                    auto nrFlights = graph.nrFlights(map[s],map[d],airlines);
                    cout << " Número mínimo de voos entre " << s << " e " << d << " : " << nrFlights << "\n\n";

                    auto usedAirports = graph.usedAirportsFlights(map[s],map[d],airlines);
                    auto usedAirlines = graph.usedAirlinesFlights(map[s],map[d],airlines);

                    if (usedAirlines.size() == 1) cout << " Existe apenas 1 trajeto possível\n";
                    else cout << " Existem " << usedAirlines.size() << " trajetos possíveis\n";

                    showPath(usedAirports,usedAirlines);
                }
            }
    }
    else{
        for (auto s : src)
            for (auto d : dest){
                if (graph.nrFlights(map[s],map[d],airlines) == -1){
                    cout << "Não existem voos de " << s << " a " << d << " com as companhias aéreas indicadas\n";
                }
                else {
                    printf("\n\n \033[40m===========================================================\033[0m\n\n");
                    auto x = graph.flownDistance(map[s],map[d],airlines);
                    cout << " Distância mínima percorrida entre " << s << " e " << d << " : " << x << " km\n\n";
                    auto usedAirports = graph.usedAirportsDistance(map[s],map[d],airlines);
                    auto usedAirlines = graph.usedAirlinesDistance(map[s],map[d],airlines);


                    if (usedAirlines.size() == 1) cout << " Existe apenas 1 trajeto possível\n";
                    else cout << " Existem " << usedAirlines.size() << " trajetos possíveis\n";

                    showPath(usedAirports,usedAirlines);
                }
            }
    }
    printf("\n \033[44m===========================================================\033[0m\n\n");
    airlines = unordered_set<Airline, Airline::AirlineHash,Airline::AirlineHash>();
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
            case 4:  break;
            case 5:  break;
            case 0: return;
        }
    }
}

void Menu::showPath(list<queue<Airport>> usedAirports, list<queue<Airline>> usedAirlines) {
    auto i = usedAirports.begin();
    auto j = usedAirlines.begin();
    for (int nrPath = 0; nrPath < usedAirports.size(); nrPath ++) {

        cout << " Trajeto nº" << nrPath+1 << " : ";

        auto airports = *i;
        auto airlines = *j;
        while (!airports.empty() && !airlines.empty()){
            printf(" \033[44m%s\033[0m",airports.front().getCode().c_str());
            cout <<" --- (";
            printf(" \033[41m%s\033[0m",airlines.front().getCode().c_str());
            cout << ") --- ";
            airports.pop();
            airlines.pop();
        }
        printf(" \033[44m%s\033[0m",airports.front().getCode().c_str());
        cout << "\n";
        i++;
        j++;
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
                string country = validateCountry();
                cout << "\n";
                for (auto i : supervisor->getAirports()){
                    if (i.getCountry() == country) cout << " " << i.getCode() << " - " << i.getName() << endl;
                }
                break;
            }
            case 3: {
                Graph graph = supervisor->getGraph();
                int choice = showTop(), top;
                if (choice == 1) top = 10;
                else if (choice == 2) top = 20;
                else if (choice == 3) top = customTop(3020);
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
                int choice = showTop(), top;
                if (choice == 1) top = 10;
                else if (choice == 2) top = 20;
                else if (choice == 3) top = customTop(3020);
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
        cout << "\n Pretende ver:\n\n "
                "[1] Companhias aéreas totais\n [2] Companhias aéreas de um país\n [3] Os voos mais longos de uma companhia aérea\n [0] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: {
                for (auto i : supervisor->getAirlines()) cout << " " << i.getCode() << " - " << i.getName() << endl;
                break;
            }
            case 2: {
                string country = validateCountry();
                cout << "\n";
                for (auto i : supervisor->getAirlines()){
                    if (i.getCountry() == country){ cout << " " << i.getCode() << " - " << i.getName() << endl;}
                }
                break;
            }
            case 3: {
                Airline airline = validateAirline();
                string airport = validateAirport();
                cout << "\n";
                pair<int, queue<Airport>> longestFlight = supervisor->getGraph().diameter(supervisor->getMap()[airport], airline);
                while (!longestFlight.second.empty()){
                    Airport a = longestFlight.second.front();
                    longestFlight.second.pop();
                    cout << " " << a.getCode() << " - ";
                }
                cout << "diâmetro: " << longestFlight.first << endl;
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
void Menu::showCountries(){
    int option;
    multimap<int,string> nrAirports = supervisor->convertMap(supervisor->getNrAirportsPerCountry());
    while(true){
        cout << "\n Pretende ver os países:\n\n "
                "[1] Com mais aeroportos\n [2] Com menos aeroportos\n [0] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: {
                int choice = showTop(), top, j = 1;
                if (choice == 1) top = 10;
                else if (choice == 2) top = 20;
                else if (choice == 3) top = customTop(226);
                else return;
                for (auto i = nrAirports.rbegin(); i != nrAirports.rend(); i++){
                    if (top == 0) break;
                    cout << "\n " << j<< ". " << i->second << " - " << i->first << " aeroportos\n";
                    top--; j++;
                }
                break;
            }
            case 2:{
                int choice = showTop(), top, j = 1;
                if (choice == 1) top = 10;
                else if (choice == 2) top = 20;
                else if (choice == 3) top = customTop(226);
                else return;
                for (auto i = nrAirports.begin(); i != nrAirports.end(); i++){
                    if (top == 0) break;
                    cout << "\n " << j << ". " << i->second << " - " << i->first << " aeroporto(s)\n";
                    top--; j++;
                }
                break;}
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
                "[1] Existem no total\n [2] Existem a partir de um aeroporto\n [0] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: cout << "\n Existem " << supervisor->nrFlights() << " voos no total\n"; break;
            case 2: break;
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
                string country = validateCountry();
            }
            case 0: return;
        }
    }
}

string Menu::validateCountry(){
    string country;

    cout << " Insira o nome do país (ex: Portugal): ";
    cin.ignore();
    getline(cin,country,'\n');

    while(cin.fail() || !supervisor->isCountry(country)) {
        cout << " Este país não existe\n";
        cout << " Insira o nome do país (ex: Portugal): ";
        cin.clear();
        getline(cin,country,'\n');
    }
    return country;
}
double Menu::validateLatitude() {
    double latitude;
    cout << " Insira o valor da latitude: "; cin >> latitude;
    while(cin.fail() || latitude < -90 || latitude > 90) {
        if (cin.fail()) cout << " Input inválido\n";
        else cout << " Insira um valor entre -90 e 90\n";
        cout << " Insira o valor da latitude: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> latitude;
    }
    return latitude;
}
double Menu::validateLongitude() {
    double longitude;
    cout << " Insira o valor da longitude: "; cin >> longitude;
    while(cin.fail() || longitude < -180 || longitude > 180) {
        if (cin.fail()) cout << " Input inválido\n";
        else cout << " Insira um valor entre -180 e 180\n";
        cout << " Insira o valor da longitude: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> longitude;
    }
    return longitude;
}
double Menu::validateRadius() {
    double radius;
    cout << " Insira o valor do raio: "; cin >> radius;
    while(cin.fail() || radius <= 0) {
        if (cin.fail()) cout << " Input inválido\n";
        else cout << " Insira um valor positivo\n";
        cout << " Insira o valor do raio:  ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> radius;
    }
    return radius;
}
string Menu::validateAirport(){
    string airport;
    cout << " Insira o código IATA do aeroporto (ex: CDG): "; cin >> airport;

    while(cin.fail() || !supervisor->isAirport(Airport(airport))) {
        if (cin.fail() || airport.size() != 3) cout << " Input inválido " << '\n';
        else cout << " Não existe nenhum aeroporto com este código " << '\n';
        cout << " Insira o código IATA do aeroporto (ex: CDG): ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> airport;
    }
    return airport;
}
Airline Menu::validateAirline() {
    string code;
    cout << " Insira o código ICAO da companhia aérea (ex: IBE): "; cin >> code;

    while(cin.fail() || !supervisor->isAirline(Airline(code)) || airlines.find(Airline(code)) != airlines.end()) {
        if (cin.fail() || code.size() != 3) cout << " Input inválido " << '\n';
        else if (!supervisor->isAirline(Airline(code))) cout << " Não existe nenhuma companhia aérea com este código " << '\n';
        else cout << " Já inseriu esta companhia aérea !\n";
        cout << " Insira o código ICAO da companhia aérea (ex: IBE): ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> code;
    }
    return Airline(code);
}
string Menu::validateCity(string country) {
    string city;
    cout << " Insira o nome da cidade (ex: Porto): ";

    getline(cin,city,'\n');
    while(cin.fail() || !supervisor->isValidCity(country,city)) {
        if (!supervisor->isCity(city)) cout << " Esta cidade não existe\n";
        else cout << " Esta cidade não pertence a este país :/\n";
        cout << " Insira o nome da cidade (ex: Porto): ";
        cin.clear();
        getline(cin,city,'\n');
    }
    return city;
}
int Menu::validateOption(const string &message) {
    int option;
    cout << message; cin >> option;
    while(cin.fail() || option < 1 || option > 2) {
        cout << " Input inválido" << '\n';
        cout << message;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> option;
    }
    return option;
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
int Menu::customTop(int n) {
    cout << "\n Selecione um valor para o seu top: ";
    int option; cin >> option;
    while (cin.fail() || option < 1 || option > n){
        cout << " Escolha um número entre 1 e " << n << "\n Tente novamente: ";
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