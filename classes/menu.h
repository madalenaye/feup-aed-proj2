//
// Created by Madalena Ye on 27/12/2022.
//

#ifndef RENAIR_MENU_H
#define RENAIR_MENU_H

#include "supervisor.h"

class Menu{
public:
    Menu();
    void init();
    void end();
    void showOperations();
    void showStatistics();
private:
    Supervisor* supervisor;
};
#endif //RENAIR_MENU_H
