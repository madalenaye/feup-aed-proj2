#include "menu.h"

//initial menu
Menu::Menu() {
    printf("\n");
    printf(" \033[44m===========================================================\033[0m\t\t");
    cout << "\n\n" << " Bem-vindo!\n (Pressione [0] sempre que quiser voltar atrás)\n\n";
    supervisor = new Supervisor();
    unordered_set<Airline,Airline::AirlineHash,Airline::AirlineHash> A;
    A.insert(Airline("TAP"));
    A.insert(Airline("SAT"));
    A.insert(Airline("ICE"));
    A.insert(Airline("GRL"));
    A.insert(Airline("FXI"));
    /*auto x = supervisor->getGraph().dijkstra(1,4,A);
    cout << x.distance << endl;*/
    vector<vector<int> > paths;
    vector<int> path;
    auto map = supervisor->getMap();
    supervisor->getGraph().printPaths(map["FLW"],map["AEY"],A);

}

//options of the menu
void Menu::init() {
    string option;
    while(true) {
        cout << " O que deseja fazer hoje?\n\n"
                " [1] Realizar operação\n [2] Consultar informação\n [3] Ver estatísticas\n [4] Sair\n\n Opção: ";

        cin >> option;

        if (option == "1")
            chooseSource();

        else if (option == "2")
            info();

        else if (option == "3")
            statistics();

        else if (option == "4")
            return;

        else if (option == "0")
            cout << "\n Não é possível voltar mais atrás!\n\n";

        else{
            cout << "\n Input inválido, tente novamente. \n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

//operations
void Menu::chooseSource() {
    string option;

    while(true){

        cout << "\n Pretende partir de: \n\n "
                "[1] Um aeroporto em específico\n [2] Uma cidade\n [3] Uma localização\n\n Opção: ";
        cin >> option;

        if (option == "1"){
            string airport = validateAirport();
            if (airport == "0") continue;

            src.clear();
            src.push_back(airport);

            chooseTarget();
            return;
        }

        if (option == "2"){
            string country = validateCountry();
            if (country == "0") continue;

            string city = validateCity(country);
            if (city == "0") continue;

            auto airportsPerCity = supervisor->getCity();
            src = airportsPerCity[{country,city}];

            chooseTarget();
            return;
        }

        if (option == "3"){
            src = validateLocal();
            chooseTarget();
            return;
        }

        if (option == "0"){
            cout << "\n";
            return;
        }

        cout << "\n Input inválido, tente novamente. \n";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
}
void Menu::chooseTarget() {

    string option;

    while(true){
        cout << "\n Pretende chegar a: \n\n "
                "[1] Um aeroporto em específico\n [2] Uma cidade\n [3] Uma localização\n\n Opção: ";
        cin >> option;
        if (option == "1"){
            string destination = validateAirport();
            if (destination == "0") continue;
            while (src.size() == 1 && destination == src.front()) {
                cout << " Não pode partir e chegar ao mesmo aeroporto num voo :(\n";
                destination = validateAirport();
            }
            dest.clear();
            dest.push_back(destination);

            chooseAirlines();
            return;
        }

        if (option == "2"){
            string country = validateCountry();
            if (country == "0") continue;

            string city = validateCity(country);
            if (city == "0") continue;

            auto airportsPerCity = supervisor->getCity();
            dest = airportsPerCity[{country,city}];

            chooseAirlines();
            return;
        }

        if (option == "3"){
            dest = validateLocal();
            chooseAirlines();
            return;
        }

        if (option == "0"){
            chooseSource();
            return;
        }

        cout << "\n Input inválido, tente novamente. \n\n";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
}
void Menu::chooseAirlines() {

    string choice = validateOption("\n Pretende escolher as companhias aéreas a usar? \n\n"
                                " [1] Sim\n [2] Não\n\n Opção: ");
    if (choice == "0"){
        chooseTarget();
        return;
    }

    if (choice == "1") {
        string airline = validateAirline();
        if (airline == "0") return;
        airlines.insert(Airline(airline));

        string option = validateOption("\n Deseja inserir mais alguma companhia aérea? \n\n"
                                    " [1] Sim\n [2] Não\n\n Opção: ");
        while (option == "1") {
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
    string option = validateOption("\n Indique o critério a usar: \n\n"
                                " [1] Número mínimo de voos\n [2] Distância mínima percorrida\n\n Opção: ");;
    while (option == "0") {
        cout << "\n Input inválido, tente novamente. \n\n";
        option = validateOption("\n Indique o critério a usar: \n\n"
                                " [1] Número mínimo de voos\n [2] Distância mínima percorrida\n\n Opção: ");
    }

    if (option == "1"){
        printf("\n\033[1m\033[32m===============================================================\033[0m\n\n");
        auto flightPath = supervisor->processFlight(src,dest,airlines);
        for (auto pair : flightPath) {
            string source = pair.first;
            string target = pair.second;
            //printPath(source,target);
            supervisor->getGraph().printPaths(map[source], map[target],airlines);
        }
        /*auto flightPath = supervisor->processFlight(src,dest,airlines);


        if (flightPath.empty())
            cout << " Não existem voos\n\n" ;
        else {
            int nrPath = 0;
            int nrFlights;
            for (const auto& path: flightPath) {
                string source = path.first;
                string target = path.second;
                nrFlights = graph.nrFlights(map[source], map[target], airlines);
                auto usedAirports = graph.usedAirportsFlights(map[source], map[target], airlines);
                auto usedAirlines = graph.usedAirlinesFlights(map[source], map[target], airlines);

                showPath(usedAirports, usedAirlines,nrPath);
            }
            if (nrPath != 1) cout << " No total, existem " << nrPath << " trajetos possíveis\n";
            else cout << " Apenas existe 1 trajeto possível\n";
            cout << " O número de voos mínimos é " << nrFlights << "\n\n";
        }*/
    }
    else{
        printf("\n\033[1m\033[35m===============================================================\033[0m\n\n");
        auto flightPath = supervisor->processDistance(src,dest,airlines);
        if (flightPath.empty())
            cout << " Não existem voos\n\n";
        else {
            int nrPath = 0;
            double distance;
            for (auto path: flightPath) {
                string source = path.first;
                string target = path.second;
                distance = graph.flownDistance(map[source], map[target], airlines);
                auto usedAirports = graph.usedAirportsDistance(map[source], map[target], airlines);
                auto usedAirlines = graph.usedAirlinesDistance(map[source], map[target], airlines);

                showPath(usedAirports, usedAirlines,nrPath);
            }
            if (nrPath != 1) cout << " No total, existem " << nrPath << " trajetos possíveis\n";
            else cout << " Apenas existe 1 trajeto possível\n";
            cout << " A distância mínima percorrida é " << distance << " km" << "\n\n";
        }
    }

    printf("\033[1m\033[36m===============================================================\033[0m\n\n");
    airlines = unordered_set<Airline, Airline::AirlineHash,Airline::AirlineHash>();
}
void Menu::showPath(list<queue<Airport>> usedAirports, list<queue<Airline>> usedAirlines, int& nrPath) {
    auto i = usedAirports.begin();
    auto j = usedAirlines.begin();
    for (int n = 0; n < usedAirports.size(); n ++) {

        cout << " Trajeto nº" << ++nrPath << ": ";

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
//info
void Menu::info(){
    string option;
    while(true){
        cout << "\n Que tipo de informação deseja ver? \n\n "
                "[1] Aeroporto específico\n [2] Aeroportos\n [3] Companhias Aéreas\n [4] Países\n\n Opção: ";
        cin >> option;
        if (option == "1")
            showAirport();

        else if (option == "2")
            showAirports();

        else if (option == "3")
            showAirlines();

        else if (option == "4")
            showCountries();

        else if (option == "0") {
            cout << "\n";
            return;
        }
        else{
            cout << "\n Input inválido, tente novamente.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::showAirport(){
    string option;

    while(true){
        cout << "\n A partir de um aeroporto, pretende ver:\n\n "
                "[1] Voos existentes\n [2] Companhias aéreas existentes\n [3] Destinos alcançáveis\n [4] Países alcançáveis\n [5] Aeroportos/cidades/países possíveis de alcançar com X voos\n\n Opção: ";
        cin >> option;
        string airport;
        int source;
        if (option == "1"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = supervisor->getMap()[airport];
            cout << "\n";
            for (const auto& i: supervisor->getGraph().getNodes()[source].adj){
                string target = supervisor->getGraph().getNodes()[i.dest].airport.getCode();
                cout << " " << airport << " ---( "<< i.airline.getCode() << " )--- " <<  target << endl;
            }
        }
        else if (option == "2"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = supervisor->getMap()[airport];
            cout << "\n";
            for (const auto& i: supervisor->getGraph().airlinesFromAirport(source)){
                printf("\033[1m\033[35m -\033[0m");
                cout << " " << i << endl;
            }
        }
        else if (option == "3"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = supervisor->getMap()[airport];
            cout << "\n";
            for (const auto& i: supervisor->getGraph().targetsFromAirport(source)){
                printf("\033[1m\033[36m %s \033[0m", i.second.c_str()) ;
                cout <<  "- "<< i.first << endl;
            }
        }
        else if (option == "4"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = supervisor->getMap()[airport];
            cout << "\n";
            for (const auto& i: supervisor->getGraph().countriesFromAirport(source)){
                printf("\033[1m\033[32m -\033[0m");
                cout << " " << i << endl;
            }
        }
        else if (option == "5"){
            airport = validateAirport();
            if (airport == "0") continue;
            showOptions(airport);
        }

        else if (option == "0")
            return;

        else{
            std::cout << "\n Input inválido, tente novamente. \n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::showOptions(const string& airport) {
    int maxFlight = customTop(" Que número máximo de voos pretende realizar: ", 63832);
    string option;
    while(true){
        cout << "\n O que pretende ver?\n\n"
                " [1] Aeroportos\n [2] Cidades\n [3] Países\n\n Opção: ";
        cin >> option;

        if (option == "1") {
            auto res = supervisor->getGraph().listAirports(supervisor->getMap()[airport], maxFlight);
            cout << "\nA partir de " << airport << " é possível alcançar o(s) seguinte(s) aeroporto(s)" << "\n\n";
            for(const auto& airports:res){
                printf("\033[1m\033[32m %s \033[0m", airports.getCode().c_str());
                cout << "- " << airports.getName() << '\n';
            }
        }
        else if (option == "2") {
            auto res = supervisor->getGraph().listCities(supervisor->getMap()[airport], maxFlight);
            cout << "\nA partir de " << airport << " é possível alcançar a(s) seguinte(s) cidades(s)" << "\n\n";
            for(const auto& city:res){
                printf("\033[1m\033[32m %s \033[0m", city.second.c_str());
                cout << "- "<< city.first << '\n';
            }
        }
        else if (option == "3"){
            auto res = supervisor->getGraph().listCountries(supervisor->getMap()[airport], maxFlight);
            cout << "\nA partir de " << airport << " é possível alcançar o(s) seguinte(s) países(s)" << "\n\n";
            for(const auto& country:res){
                printf("\033[1m\033[32m - \033[0m");
                cout << " " << country << '\n';
            }
        }
        else if (option == "0")
            return;

        else {
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::showAirports(){
    string option;
    while(true){
        cout << "\n Pretende ver os aeroportos:\n\n "
                "[1] Totais\n [2] De um país\n [3] Com mais voos\n [4] Com mais companhias aéreas\n\n Opção: ";
        cin >> option;
        if (option == "1")
            for (const auto& i: supervisor->getAirports()) {
                printf("\033[1m\033[32m %s\033[0m", i.getCode().c_str());
                cout << " - " << i.getName() << endl;
            }

        else if (option == "2"){
            string country = validateCountry();
            if (country == "0") continue;
            cout << "\n";
            for (const auto& i : supervisor->getAirports())
                if (i.getCountry() == country) {
                    printf("\033[1m\033[35m %s\033[0m", i.getCode().c_str());
                    cout << " - " << i.getName() << endl;
                }
        }
        else if (option == "3"){
            Graph graph = supervisor->getGraph();
            int choice = showTop(), top;
            if (choice == 1) top = 10;
            else if (choice == 2) top = 20;
            else if (choice == 3) top = customTop("\n Selecione um valor para o top: ", 3020);
            else continue;
            int j = 1;
            for (int i = 0; i < top; i++){
                printf("\n\033[1m\033[36m %i\033[0m", j);
                cout << ". " << graph.flightsPerAirport()[i].second
                     << " - " << graph.flightsPerAirport()[i].first << " voos\n";
                j++;
            }
        }
        else if (option == "4"){
            Graph graph = supervisor->getGraph();
            int choice = showTop(), top;
            if (choice == 1) top = 10;
            else if (choice == 2) top = 20;
            else if (choice == 3) top = customTop("\n Selecione um valor para o top: ",3020);
            else continue;
            int j = 1;
            for (int i = 0; i < top; i++){
                printf("\n\033[1m\033[32m %i\033[0m", j);
                cout<< ". " << graph.airlinesPerAirport()[i].second
                    << " - " << graph.airlinesPerAirport()[i].first << " companhias aéreas\n";
                j++;
            }
        }
        else if (option == "0")
            return;
        else {
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::showAirlines(){
    string option;
    while(true){
        cout << "\n Pretende ver:\n\n "
                "[1] Companhias aéreas totais\n [2] Companhias aéreas de um país\n [3] O voo mais longo de uma companhia aérea\n\n Opção: ";
        cin >> option;
        if (option == "1")
            for (auto i : supervisor->getAirlines()){
                printf("\033[1m\033[36m %s\033[0m", i.getCode().c_str());
                cout << " - " << i.getName() << endl;
            }

        else if (option == "2"){
            string country = validateCountry();
            if (country == "0") continue;
            cout << "\n";
            for (auto i : supervisor->getAirlines())
                if (i.getCountry() == country){
                    printf("\033[1m\033[32m %s\033[0m", i.getCode().c_str());
                    cout << " - " << i.getName() << endl;
                }
        }

        else if (option == "3"){
            string airline = validateAirline();
            if (airline == "0") continue;
            cout << "\n";
            auto longest = supervisor->getGraph().longestFlight(Airline(airline));
            int nrFlights = longest.size()-1;
            while (!longest.empty()){
                Airport airport = longest.top();
                printf(" \033[1m\033[44m %s \033[0m", airport.getCode().c_str());
                cout << " - ";
                longest.pop();
            }
            printf(" \033[1m\033[42mnº de voos: %i \033[0m " ,nrFlights);
            cout << "\n";
        }
        else if (option == "0")
            return;
        else {
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::showCountries(){
    string option;
    multimap<int,string> nrAirports = supervisor->convertMap(supervisor->getNrAirportsPerCountry());
    while(true){
        cout << "\n Pretende ver os países:\n\n "
                "[1] Com mais aeroportos\n [2] Com menos aeroportos\n\n Opção: ";
        cin >> option;
        if (option == "1"){
            int choice = showTop(), top, j = 1;
            if (choice == 1) top = 10;
            else if (choice == 2) top = 20;
            else if (choice == 3) top = customTop("\n Selecione um valor para o top: ", 226);
            else continue;
            for (auto i = nrAirports.rbegin(); i != nrAirports.rend(); i++){
                if (top == 0) break;
                printf("\n\033[1m\033[36m %i\033[0m", j);
                cout << ". " << i->second << " - " << i->first << " aeroportos\n";
                top--; j++;
            }
        }
        else if (option == "2") {
            int choice = showTop(), top, j = 1;
            if (choice == 1) top = 10;
            else if (choice == 2) top = 20;
            else if (choice == 3) { top = customTop("\n Selecione um valor para o top: ", 226);}
            else continue;
            for (auto &nrAirport: nrAirports) {
                if (top == 0) break;
                printf("\n\033[1m\033[32m %i\033[0m", j);
                cout << ". " << nrAirport.second << " - " << nrAirport.first << " aeroporto(s)\n";
                top--;
                j++;
            }
        }
        else if (option == "0")
            return;

        else {
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

//statistics
void Menu::statistics() {
    string option;
    while(true){
        cout << "\n Que dados pretende analisar? \n\n "
                "[1] Estatísticas de um aeroporto\n [2] Nº de voos\n [3] Nº de aeroportos \n [4] Nº de companhias\n\n Opção: ";

        cin >> option;
        if (option == "1")
            airportStats();

        else if (option == "2")
            numberFlights();

        else if (option == "3")
            numberAirports();

        else if (option == "4")
            numberAirlines();

        else if (option == "5"){
            string airport = validateAirport();
            if (airport == "0") continue;
            int y = customTop(" Selecione um valor máximo: ", 63832);
            if (y == 0) continue;
            int count = supervisor->getGraph().listCountries(supervisor->getMap()[airport], y).size();
            cout << "\n A partir de " << airport << " é possível atingir " << count << " países usando um máximo de "
                 << y << " voos\n";
        }

        else if (option == "6") {
            string airport = validateAirport();
            if (airport == "0") continue;
            int y = customTop(" Selecione um valor máximo: ", 63832);
            if (y == 0) continue;
            int count = supervisor->getGraph().listCities(supervisor->getMap()[airport], y).size();
            cout << "\n A partir de " << airport << " é possível atingir " << count << " cidades usando um máximo de "
                 << y << " voos\n";
        }

        else if (option == "0") {
            cout << "\n";
            return;
        }

        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::airportStats() {
    string option;
    while(true){
        cout << "\n A partir de um aeroporto, pretende saber: \n\n"
            " [1] Nº de voos existentes\n [2] Nº de companhias aéreas\n [3] Nº de destinos alcançáveis\n"
            " [4] Nº de países atíngiveis\n [5] Nº de aeroportos/cidades/países possíveis de alcançar com um máximo de X voos"
            "\n\n Opção: ";
        cin >> option;
        string airport;
        int source;

        if (option == "1"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = supervisor->getMap()[airport];
            cout << "\n Nº de voos existentes a partir de " << airport << " : ";
            printf("\033[1m\033[36m %lu \n\033[0m", supervisor->getGraph().listAirports(source,1).size()) ;
        }

        else if (option == "2"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = supervisor->getMap()[airport];
            cout << "\n Nº de companhias aéreas de " << airport << " : ";
            printf("\033[1m\033[36m %lu \n\033[0m", supervisor->getGraph().airlinesFromAirport(source).size());
        }

        else if (option == "3"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = supervisor->getMap()[airport];
            cout << "\n Nº de destinos distintos alcancáveis a partir de " << airport << " : ";
            printf("\033[1m\033[36m %lu \n\033[0m", supervisor->getGraph().targetsFromAirport(source).size());
        }

        else if (option == "4"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = supervisor->getMap()[airport];
            cout << "\n Nº de países diferentes alcancáveis a partir de " << airport << " : ";
            printf("\033[1m\033[36m %lu \n\033[0m", supervisor->getGraph().countriesFromAirport(source).size());
        }
        else if (option == "5")
            maxReach();

        else if (option == "0")
            return;

        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::maxReach() {
    string option;
    string airport = validateAirport();
    if (airport == "0") return;
    int maxFlight = customTop(" Que número máximo de voos pretende realizar: ", 63832);
    while(true){
        cout << "\n O que pretende ver?\n\n"
                " [1] Aeroportos\n [2] Cidades\n [3] Países\n\n Opção: ";
        cin >> option;

        if (option == "1") {
            auto res = supervisor->getGraph().listAirports(supervisor->getMap()[airport], maxFlight);
            cout << "\n A partir de " << airport << " é possível alcançar ";
            printf("\033[1m\033[35m%lu \033[0m", res.size());
            cout << "aeroportos\n";
        }
        else if (option == "2") {
            auto res = supervisor->getGraph().listCities(supervisor->getMap()[airport], maxFlight);
            cout << "\n A partir de " << airport << " é possível alcançar ";
            printf("\033[1m\033[32m%lu \033[0m", res.size());
            cout << "cidades\n";
        }
        else if (option == "3"){
            auto res = supervisor->getGraph().listCountries(supervisor->getMap()[airport], maxFlight);
            cout << "\n A partir de " << airport << " é possível alcançar ";
            printf("\033[1m\033[34m%lu \033[0m", res.size());
            cout << "países\n";
        }
        else if (option == "0")
            return;

        else {
            std::cout << "\n Input inválido, tente novamente. \n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::numberFlights(){
    string option;
    while(true){
        cout << "\n Pretende saber quantos voos:\n\n "
                "[1] Existem no total\n [2] Existem a partir de um aeroporto\n\n Opção: ";
        cin >> option;
        if (option == "1"){
            cout << "\n Existem " << supervisor->nrFlights() << " voos no total\n";
        }
        else if (option == "2"){
            string code = validateAirport();
            if (code == "0") continue;
            Airport airport = *supervisor->getAirports().find(Airport(code));
            int node = supervisor->getMap()[code];
            cout << "\n Existem " << supervisor->getGraph().getNodes()[node].adj.size() << " voos a partir de " << airport.getName() << '\n';
        }
        else if (option == "0")
            return;
        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::numberAirports() {
    string option;
    while(true){
        cout << "\n Pretende saber quantos aeroportos:\n\n "
                "[1] Existem no total\n [2] Existem num país\n [3] São atingíveis usando um máximo de Y voos\n\n Opção: ";
        cin >> option;
        if (option == "1")
            cout << "\n Nº de aeroportos totais: " << supervisor->getAirports().size() << endl;

        else if (option == "2"){
            string country = validateCountry();
            if (country == "0") continue;
            int count = supervisor->getNrAirportsPerCountry()[country];

            cout << "\n Neste país (" << country << ") existem " << count << " aeroportos\n\n";
        }

        else if (option == "3"){

            string airport = validateAirport();
            if (airport == "0") continue;
            int y = customTop("\n Selecione um valor para o máximo: ", 63832);
            if (y == 0) continue;
            int count = supervisor->getGraph().listAirports(supervisor->getMap()[airport], y).size();
            cout << "\n A partir de " << airport << " é possível atingir " << count << " aeroportos usando um máximo de "
                 << y << " voos\n";

        }

        else if (option == "0")
            return;

        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::numberAirlines() {
    string option;
    while(true){
        cout << "\n Pretende ver o número de:\n\n "
                "[1] Companhias aéreas totais\n [2] Companhias aéreas de um país\n\n Opção: ";
        cin >> option;
        if (option == "1")
            cout << "\n Nº de companhias aéreas totais: " << supervisor->getAirlines().size() << '\n';

        else if (option == "2") {
            string country = validateCountry();
            if (country == "0") continue;
            cout << "\n Nº de companhias aéreas : " << supervisor->countAirlinesPerCountry(country) << '\n';
        }
        else if (option == "0")
            return;

        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}


//validate user input
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

    while(!supervisor->isCountry(country)) {
        if (country == "0") return "0";
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
        cout << " Insira o valor do raio: ";
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
        if (airline == "0") return "0";
        if (cin.fail() || airline.size() != 3) cout << " Input inválido " << '\n';
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

    while(cin.fail() || !supervisor->isValidCity(country,city)) {
        if (city == "0") return "0";
        if (!supervisor->isCity(city)) cout << " Esta cidade não existe\n";
        else cout << " Esta cidade não pertence a este país :/\n";
        cout << " Insira o nome da cidade (ex: Porto): ";
        cin.clear();
        getline(cin,city,'\n');
    }
    return city;
}
string Menu::validateOption(const string &message) {
    string option;
    cout << message; cin >> option;
    while(!(option == "0" || option == "1" || option == "2")) {
        cout << "\n Input inválido" << '\n';
        cout << message;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> option;
    }
    return option;
}
int Menu::showTop(){
    cout << "\n Deseja consultar:\n\n "
            "[1] Top 10\n [2] Top 20\n [3] Outro\n\n Opção: ";
    int option;
    cin >> option;
    while (cin.fail() || option < 0  || option > 4){
        cout << " Input inválido\n Tente novamente: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> option;
    }
    return option;
}
//custom user input
int Menu::customTop(const string& message, int n) {
    cout << message;
    int option; cin >> option;
    while (cin.fail() || option < 0 || option > n){
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