//
// Created by Madalena Ye on 27/12/2022.
//
#include "Menu.h"

//apresentação do menu
Menu::Menu() {
    printf("\n");
    printf("\033[44m===========================================================\033[0m\t\t");
    cout << "\n\n" << " Bem-vindo!" << endl;
    supervisor = new Supervisor();
}

void Menu::init() {
    int option;
    while(true){
    cout << " O que deseja fazer hoje?\n" << "\n" << " [1] Realizar operação " << "\n" << " [2] Ver estatísticas" << "\n"<< " [3] Sair\n" << "\n Opção: ";
    cin >> option;
    switch (option) {
        case 1 : //showOperations(); break;
        case 2 : //showStatistics(); break;
        case 3 : {end(); return;}
        default: {
            cout << "\n Input inválido, tente novamente\n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            break;
        }
    }
    }
}
void Menu::end() {
    printf("\n");
    printf("\033[46m===========================================================\033[0m\n");
}