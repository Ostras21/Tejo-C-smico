cmake_minimum_required(VERSION 3.16)
project(TejoCosmico LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(TejoCosmico
    main.cpp
    juego.cpp
    juego.h
    tejo.cpp
    tejo.h
    recursos.qrc
)

target_link_libraries(TejoCosmico PRIVATE Qt6::Widgets)
