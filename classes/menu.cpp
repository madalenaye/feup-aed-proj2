#include <climits>
#include "menu.h"

//initial menu
Menu::Menu() {
    printf("\n");
    printf(" \033[44m===========================================================\033[0m\t\t");
    cout << "\n\n" << " Bem-vindo!\n (Pressione [0] sempre que quiser voltar atrás)\n" << endl;
    supervisor = new Supervisor();
}

//options of the menu
void Menu::init() {
    int option;
    while(true) {
        cout << " O que deseja fazer hoje?\n\n"
                " [1] Realizar operação\n [2] Consultar informação\n [3] Ver estatísticas\n [4] Sair\n\n Opção: ";

        cin >> option;

        if (option == 1)
            chooseSource();

        else if (option == 2)
            showInfo();

        else if (option == 3)
            showStatistics();

        else if (option == 4){
            end(); return;
        }
        else if (!cin.fail() && option == 0)
        {cout << " Não é possível voltar mais atrás! ";return;}

        else{
            cout << "\n Input inválido, tente novamente. \n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

void Menu::chooseSource() {
    int option;

    while(true){

        cout << "\n Pretende partir de: \n\n "
                "[1] Um aeroporto em específico\n [2] Uma cidade\n [3] Uma localização\n\n Opção: ";
        cin >> option;
        if (option == 1){
            string airport = validateAirport();
            if (airport == "0") return;

            src.clear();
            src.push_back(airport);

            chooseTarget();
            return;
        }

        if (option == 2){
            string country = validateCountry();
            if (country == "0") return;

            string city = validateCity(country);
            if (city == "0") return;

            auto airportsPerCity = supervisor->getCity();
            src = airportsPerCity[{country,city}];

            chooseTarget();
            return;
        }

        if (option == 3){
            src = validateLocal();
            chooseTarget();
            return;
        }

        if (option == 0)
            return;

        cout << "\n Input inválido, tente novamente. \n\n";
        cin.clear();
        cin.ignore(INT_MAX, '\n');

    }
}
void Menu::chooseTarget() {

    int option;

    while(true){
        cout << "\n Pretende chegar a: \n\n "
                "[1] Um aeroporto em específico\n [2] Uma cidade\n [3] Uma localização\n\n Opção: ";
        cin >> option;
        if (option == 1){
            string destination = validateAirport();
            if (destination == "0") return;
            while (src.size()==1 && destination == src.front()) {
                cout << " Não pode partir e chegar ao mesmo aeroporto num voo :(\n";
                destination = validateAirport();
            }
            dest.clear();
            dest.push_back(destination);

            chooseAirlines();
            return;
        }

        if (option == 2){
            string country = validateCountry();
            if (country == "0") return;

            string city = validateCity(country);
            if (city == "0") return;

            auto airportsPerCity = supervisor->getCity();
            dest = airportsPerCity[{country,city}];

            chooseAirlines();
            return;
        }

        if (option == 3){
            dest = validateLocal();
            chooseAirlines();
            return;
        }

        if (option == 0){
            chooseSource();
            return;}

        cout << "\n Input inválido, tente novamente. \n\n";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
}
void Menu::chooseAirlines() {

    int choice = validateOption("\n Pretende escolher as companhias aéreas a usar? \n\n"
                                " [1] Sim\n [2] Não\n\n Opção: ");
    if (choice == 0){
        chooseTarget();
        return;
    }
    if (choice == 1) {
        string airline = validateAirline();
        if (airline == "0") return;
        airlines.insert(Airline(airline));

        int option = validateOption("\n Deseja inserir mais alguma companhia aérea? \n\n"
                                    " [1] Sim\n [2] Não\n\n Opção: ");
        while (option == 1) {
            airline = validateAirline();
            airlines.insert(Airline(airline));
            option = validateOption("\n Deseja inserir mais alguma companhia aérea? \n\n"
                                    " [1] Sim\n [2] Não\n\n Opção: ");
        }
    }
    processOperation();
}

void Menu::processOperation() {
    Graph graph = supervisor->getGraph();
    auto map = supervisor->getMap();
    int option = validateOption("\n Indique o critério a usar: \n\n"
                               " [1] Número mínimo de voos\n [2] Distância mínima percorrida\n\n Opção: ");;
    while (option == 0) {
        cout << "\n Input inválido, tente novamente. \n\n";
        option = validateOption("\n Indique o critério a usar: \n\n"
                                " [1] Número mínimo de voos\n [2] Distância mínima percorrida\n\n Opção: ");
    }
    if (option == 1){

        auto aa = supervisor->processFlight(src,dest,airlines);
        for (auto c : aa) {
            auto nrFlights = graph.nrFlights(map[c.first], map[c.second], airlines);
            cout << " Número mínimo de voos entre " << c.first << " e " << c.second << ": " << nrFlights << "\n\n";

            auto usedAirports = graph.usedAirportsFlights(map[c.first], map[c.second], airlines);
            auto usedAirlines = graph.usedAirlinesFlights(map[c.first], map[c.second], airlines);

            if (usedAirlines.size() == 1) cout << " Existe apenas 1 trajeto possível\n";
            else cout << " Existem " << usedAirlines.size() << " trajetos possíveis\n";

            showPath(usedAirports, usedAirlines);
        }
        /*
        for (const auto& s : src)
            for (const auto& d : dest){
                if (src == dest)
                    continue;
                printf("\n\n\033[1m\033[32m===========================================================\033[0m\n\n");
                if (graph.nrFlights(map[s],map[d],airlines) == -1)
                    cout << " Não existem voos de " << s << " a " << d << " com as companhias aéreas indicadas\n";
                else {
                    auto nrFlights = graph.nrFlights(map[s],map[d],airlines);
                    cout << " Número mínimo de voos entre " << s << " e " << d << ": " << nrFlights << "\n\n";

                    auto usedAirports = graph.usedAirportsFlights(map[s],map[d],airlines);
                    auto usedAirlines = graph.usedAirlinesFlights(map[s],map[d],airlines);

                    if (usedAirlines.size() == 1) cout << " Existe apenas 1 trajeto possível\n";
                    else cout << " Existem " << usedAirlines.size() << " trajetos possíveis\n";

                    showPath(usedAirports,usedAirlines);
                }
            }*/
    }
    else{
        auto aa = supervisor->processDistance(src,dest,airlines);
        for (auto c : aa) {
            auto x = graph.flownDistance(map[c.first], map[c.second], airlines);
            cout << " Distância mínima percorrida entre " << c.first << " e " << c.second << ": " << x << "\n\n";

            auto usedAirports = graph.usedAirportsFlights(map[c.first], map[c.second], airlines);
            auto usedAirlines = graph.usedAirlinesFlights(map[c.first], map[c.second], airlines);

            if (usedAirlines.size() == 1) cout << " Existe apenas 1 trajeto possível\n";
            else cout << " Existem " << usedAirlines.size() << " trajetos possíveis\n";

            showPath(usedAirports, usedAirlines);
        }
        /*
        for (const auto& s : src)
            for (const auto& d : dest){
                if (graph.nrFlights(map[s],map[d],airlines) == -1){
                    cout << "Não existem voos de " << s << " a " << d << " com as companhias aéreas indicadas\n";
                }
                else {
                    printf("\n\n\033[1m\033[34m===========================================================\033[0m\n\n");
                    auto x = graph.flownDistance(map[s],map[d],airlines);
                    cout << " Distância mínima percorrida entre " << s << " e " << d << ": " << x << " km\n\n";
                    auto usedAirports = graph.usedAirportsDistance(map[s],map[d],airlines);
                    auto usedAirlines = graph.usedAirlinesDistance(map[s],map[d],airlines);


                    if (usedAirlines.size() == 1) cout << " Existe apenas 1 trajeto possível\n";
                    else cout << " Existem " << usedAirlines.size() << " trajetos possíveis\n";

                    showPath(usedAirports,usedAirlines);
                }
            }*/
    }
    printf("\033[1m\033[36m===========================================================\033[0m\n\n");
    airlines = unordered_set<Airline, Airline::AirlineHash,Airline::AirlineHash>();
}


void Menu::showInfo(){
    int option;
    while(true){
        cout << "\n Que tipo de informação deseja ver? \n\n "
                "[1] Aeroportos\n [2] Companhias Aéreas\n [3] Países\n [4] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: showAirports(); break;
            case 2: showAirlines(); break;
            case 3: showCountries(); break;
            case 4: cout<< "\n"; return;
            default:{
                cout << "\n Input inválido, tente novamente. \n\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
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
                "[5] Nº de cidades atingíveis usando um máximo de Y voos\n [6] Voltar\n\n Opção: ";

        cin >> option;
        switch(option){
            case 1: numberFlights(); break;
            case 2: numberAirports(); break;
            case 3:  break;
            case 4: {
                string airport = validateAirport();
                int y = customTop(" Selecione um valor máximo: ", 63832);
                int count = supervisor->getGraph().countCountries(supervisor->getMap()[airport], y);
                cout << "\n A partir de " << airport << " é possível atingir " << count << " países usando um máximo de "
                     << y << " voos\n";
                break;
            }
            case 5:{
                string airport = validateAirport();
                int y = customTop(" Selecione um valor máximo: ", 63832);
                int count = supervisor->getGraph().countCities(supervisor->getMap()[airport], y);
                cout << "\n A partir de " << airport << " é possível atingir " << count << " cidades usando um máximo de "
                     << y << " voos\n";
                break;
            }
            case 6: return;
            default:{
                std::cout << "\n Input inválido, tente novamente. \n\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                break;
            }
        }
    }
}

void Menu::showPath(list<queue<Airport>> usedAirports, list<queue<Airline>> usedAirlines) {
    auto i = usedAirports.begin();
    auto j = usedAirlines.begin();
    cout << "\n";
    for (int nrPath = 0; nrPath < usedAirports.size(); nrPath ++) {

        cout << " Trajeto nº" << nrPath+1 << ": ";

        auto airports = *i;
        auto airlines = *j;
        while (!airports.empty() && !airlines.empty()){
            printf("\033[1m\033[46m %s \033[0m",airports.front().getCode().c_str());
            cout <<" --- (";
            printf("\033[1m\033[32m %s \033[0m",airlines.front().getCode().c_str());
            cout << ") --- ";
            airports.pop();
            airlines.pop();
        }
        printf("\033[1m\033[46m %s \033[0m",airports.front().getCode().c_str());
        cout << "\n\n";
        i++;
        j++;
    }
}
void Menu::showAirports(){
    int option;
    while(true){
        cout << "\n Pretende ver os aeroportos:\n\n "
                "[1] Totais\n [2] De um país\n [3] Com mais voos\n [4] Com mais companhias aéreas\n [5] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: {
                for (const auto& i: supervisor->getAirports()) cout << " " << i.getCode() << " - " << i.getName() << endl;
                break;
            }
            case 2: {
                string country = validateCountry();
                cout << "\n";
                for (const auto& i : supervisor->getAirports()){
                    if (i.getCountry() == country) cout << " " << i.getCode() << " - " << i.getName() << endl;
                }
                break;
            }
            case 3: {
                Graph graph = supervisor->getGraph();
                int choice = showTop(), top;
                if (choice == 1) top = 10;
                else if (choice == 2) top = 20;
                else if (choice == 3) top = customTop("\n Selecione um valor para o top: ", 3020);
                else return;
                int j = 1;
                for (int i = 0; i < top; i++){
                    printf("\n\033[1m\033[36m %i\033[0m", j);
                    cout << ". " << graph.flightsPerAirport()[i].second
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
                else if (choice == 3) top = customTop("\n Selecione um valor para o top: ",3020);
                else return;
                int j = 1;
                for (int i = 0; i < top; i++){
                    printf("\n\033[1m\033[32m %i\033[0m", j);
                    cout<< ". " << graph.airlinesPerAirport()[i].second
                         << " - " << graph.airlinesPerAirport()[i].first << " companhias aéreas\n";
                    j++;
                }
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
        cout << "\n Pretende ver:\n\n "
                "[1] Companhias aéreas totais\n [2] Companhias aéreas de um país\n [3] O voo mais longo de uma companhia aérea\n [4] Voltar\n\n Opção: ";
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
                string airline = validateAirline();
                string airport = validateAirport();
                cout << "\n";
                pair<int, queue<Airport>> longestFlight = supervisor->getGraph().diameter(supervisor->getMap()[airport], Airline(airline));
                while (!longestFlight.second.empty()){
                    Airport a = longestFlight.second.front();
                    string code = a.getCode();
                    longestFlight.second.pop();
                    printf(" \033[1m\033[44m %s \033[0m",a.getCode().c_str());
                    cout << " - ";
                }
                printf(" \033[1m\033[42m diâmetro: %i \033[0m " , longestFlight.first);
                cout << "\n";
                break;
            }
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
void Menu::showCountries(){
    int option;
    multimap<int,string> nrAirports = supervisor->convertMap(supervisor->getNrAirportsPerCountry());
    while(true){
        cout << "\n Pretende ver os países:\n\n "
                "[1] Com mais aeroportos\n [2] Com menos aeroportos\n [8] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: {
                int choice = showTop(), top, j = 1;
                if (choice == 1) top = 10;
                else if (choice == 2) top = 20;
                else if (choice == 3) top = customTop("\n Selecione um valor para o top: ", 226);
                else return;
                for (auto i = nrAirports.rbegin(); i != nrAirports.rend(); i++){
                    if (top == 0) break;
                    printf("\n\033[1m\033[36m %i\033[0m", j);
                    cout << ". " << i->second << " - " << i->first << " aeroportos\n";
                    top--; j++;
                }
                break;
            }
            case 2:{
                int choice = showTop(), top, j = 1;
                if (choice == 1) top = 10;
                else if (choice == 2) top = 20;
                else if (choice == 3) top = customTop("\n Selecione um valor para o top: ", 226);
                else return;
                for (auto & nrAirport : nrAirports){
                    if (top == 0) break;
                    printf("\n\033[1m\033[32m %i\033[0m", j);
                    cout << ". " << nrAirport.second << " - " << nrAirport.first << " aeroporto(s)\n";
                    top--; j++;
                }
                break;}
            case 8: return;
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
                "[1] Existem no total\n [2] Existem a partir de um aeroporto\n [3] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: cout << "\n Existem " << supervisor->nrFlights() << " voos no total\n"; break;
            case 2: {
                string code = validateAirport();
                Airport airport = *supervisor->getAirports().find(Airport(code));
                int node = supervisor->getMap()[code];
                cout << "\n Existem " << supervisor->getGraph().getNodes()[node].adj.size() << " voos a partir de " << airport.getName() << '\n';
                break;
            }
            case 3: return;
            default:
                cout << "\n Input inválido, tente novamente. \n\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                break;
        }
    }
}

void Menu::numberAirports() {
    int option;
    while(true){
        cout << "\n Pretende saber quantos aeroportos:\n\n "
                "[1] Existem no total\n [2] Existem num país\n [3] São atingíveis usando um máximo de Y voos\n [4] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: cout << "\n Nº de aeroportos totais: " << supervisor->getAirports().size() << endl; break;
            case 2: {
                string country = validateCountry();
                int count = supervisor->getNrAirportsPerCountry()[country];
                cout << "\n Neste país (" << country << ") existem " << count << " aeroportos\n\n";
            }
            case 3: {
                string airport = validateAirport();
                int y = customTop("\n Selecione um valor para o máximo: ", 63832);
                int count = supervisor->getGraph().countAirports(supervisor->getMap()[airport], y);
                cout << "\n A partir de " << airport << " é possível atingir " << count << " aeroportos usando um máximo de "
                << y << " voos\n";
                break;
            }
            case 4: return;
            default:{
                cout << "\n Input inválido, tente novamente. \n\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                break;
            }
        }
    }
}
void Menu::numberAirlines() {
    int option;
    while(true){
        cout << "\n Pretende ver o número de:\n\n "
                "[1] Companhias aéreas totais\n [2] Companhias aéreas de um país\n [8] Voltar\n\n Opção: ";
        cin >> option;
        switch (option) {
            case 1: cout << "\n Nº de companhias aéreas totais: " << supervisor->getAirlines().size() << endl; break;
            case 2: {
                string country = validateCountry();
                cout << "\n Nº de companhias aéreas : " << supervisor->countAirlinesPerCountry(country) << '\n';
                break;
            }
            case 0: return;
        }
    }
}

vector<string> Menu::validateLocal() {
    double latitude = validateLatitude();
    double longitude = validateLongitude();
    double radius = validateRadius();
    vector<string> local = supervisor->localAirports(latitude,longitude,radius);
    while (local.empty()){
        cout << " Não existem aeroportos no local indicado, tente novamente\n";
        latitude = validateLatitude();
        longitude = validateLongitude();
        radius = validateRadius();
        local = supervisor->localAirports(latitude,longitude,radius);
    }
    return local;
}
string Menu::validateCountry(){
    string country;

    cout << " Insira o nome do país (ex: Portugal): ";
    cin.ignore();

    getline(cin,country,'\n');
    if (country == "0") return "0";
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
        if (airport == "0") return "0";
        if (cin.fail() || airport.size() != 3) cout << " Input inválido " << '\n';
        else cout << " Não existe nenhum aeroporto com este código " << '\n';
        cout << " Insira o código IATA do aeroporto (ex: CDG): ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> airport;
    }
    return airport;
}

string Menu::validateAirline() {
    string airline;
    cout << " Insira o código ICAO da companhia aérea (ex: IBE): "; cin >> airline;

    while(cin.fail() || !supervisor->isAirline(Airline(airline)) || airlines.find(Airline(airline)) != airlines.end()) {
        if (cin.fail() || airline.size() != 3) cout << " Input inválido " << '\n';
        if (airline == "0") return "0";
        else if (!supervisor->isAirline(Airline(airline))) cout << " Não existe nenhuma companhia aérea com este código " << '\n';
        else cout << " Já inseriu esta companhia aérea !\n";
        cout << " Insira o código ICAO da companhia aérea (ex: IBE): ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> airline;
    }
    return airline;
}
string Menu::validateCity(const string& country) {
    string city;
    cout << " Insira o nome da cidade (ex: Porto): ";

    getline(cin,city,'\n');
    if (city == "0") return "0";

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
    while(cin.fail() || option < 0 || option > 2) {
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
int Menu::customTop(const string& message, int n) {
    cout << message;
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