cmake_minimum_required(VERSION 4.0)
project(projet_alsi53_cdataframe)

set(CMAKE_CXX_STANDARD 20)

add_executable(projet_alsi53_cdataframe src/main.cpp
        src/Column.cpp
        include/Column.h
        src/CDataFrame.cpp
        include/CDataFrame.h)

target_include_directories(projet_alsi53_cdataframe PUBLIC include)
