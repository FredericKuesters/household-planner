cmake_minimum_required(VERSION 4.3)
project(Haushaltsplaner)

set(CMAKE_CXX_STANDARD 20)

add_executable(Haushaltsplaner main.cpp
        Präsentation/Haushaltsplaner.h
        Präsentation/KonsolenMenü.cpp
        Engine/HaushaltsplanerEngine.h
        Engine/HaushaltsplanerEngine.cpp
        Engine/DashboardLogik.h
        Engine/DashboardLogik.cpp
        Präsentation/DashboardUI.h
        Präsentation/DashboardUI.cpp)
