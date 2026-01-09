cmake_minimum_required(VERSION 4.0)
project(projet_alsi53_cdataframe)

set(CMAKE_CXX_STANDARD 20)

add_executable(projet_alsi53_cdataframe main.cpp
        Column.cpp
        Column.h
        CDataFrame.cpp
        CDataFrame.h)
