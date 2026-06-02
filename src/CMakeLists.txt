cmake_minimum_required(VERSION 3.16)
project(TejoCosmico LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(TejoCosmico
    main.cpp
    comandante.cpp
    comandante.h
    enemigo.cpp
    enemigo.h
    juego.cpp
    juego.h
    mecha.cpp
    mecha.h
    roca.cpp
    roca.h
    tejo.cpp
    tejo.h
    recursos.qrc
)

target_link_libraries(TejoCosmico PRIVATE Qt6::Widgets)
