cmake_minimum_required(VERSION 4.0)
project(P2_AE2)

set(CMAKE_CXX_STANDARD 20)

add_executable(P2_AED2 solucionBacktracking.cpp
        solucionBacktracking.h
        solucionBacktracking_noPoda.h
        tiempos_BT.cpp)
add_executable(P2_AED2_SIN_PODA solucionBacktracking_noPoda.cpp)

add_executable(TIEMPOS  tiempos_BT.cpp)
