cmake_minimum_required(VERSION 3.23)
project(Renair)

set(CMAKE_CXX_STANDARD 14)

add_executable(Renair
        main.cpp classes/airport.cpp classes/airport.h classes/airline.cpp classes/airline.h classes/graph.cpp classes/graph.h classes/menu.cpp classes/menu.h classes/supervisor.cpp classes/supervisor.h)
