cmake_minimum_required(VERSION 3.31)
project(aula_7_arvore_rubro_negra)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(aula_7_arvore_rubro_negra
        arvore.h
        cmapa.h
        main.c
        mapa.c
        mapa.h)
